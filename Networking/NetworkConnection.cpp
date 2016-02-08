//=================================================================================
// NetworkConnection.cpp
// Author: Tyler George
// Date  : February 3, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Networking/NetworkConnection.hpp"
#include "Engine/Networking/NetworkSession.hpp"

////===========================================================================================
///===========================================================================================
// NetworkConnection class
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Constructors/Destructors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkConnection::NetworkConnection( const NetworkAddress& address, NetworkSession* owningSession )
    : m_nextAckID( 0 )
    , m_lastSendTime( 0.0f )
    , m_address( address )
    , m_owningSession( owningSession )
{

}
    
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkConnection::~NetworkConnection()
{

}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::SendNetworkMessage( NetworkMessage* msg )
{
    m_outgoingMessages.push_back( msg );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::Update()
{
    NetworkPacket* packet = new NetworkPacket();

    packet->SetAddress( &m_address );

    uint16_t ackID = m_nextAckID++;
    packet->WriteBytes( (void*)&ackID, sizeof( uint16_t ) );

    size_t totalRemainingSize = PACKET_MTU - (sizeof( uint16_t ) + sizeof( uint8_t ));
    size_t msgSize = 0;
    unsigned char* data = new unsigned char[ totalRemainingSize ];

    uint8_t numMsgs = 0;

    for (NetworkMessages::iterator msgIter = m_outgoingMessages.begin(); msgIter != m_outgoingMessages.end(); )
    {
        NetworkMessage* msg = *msgIter;
        size_t msgLen = msg->GetLength();
        if (msgLen <= totalRemainingSize)
        {
            numMsgs++;
            memcpy( data + msgSize, msg->m_buf, msgLen );
            totalRemainingSize -= msgLen;
            msgSize += msgLen;

            // cleanup
            delete msg;
            msgIter = m_outgoingMessages.erase( msgIter );
        }
        else
            break;
    }

    if (numMsgs > 0)
    {
        packet->WriteBytes( (void*)&numMsgs, sizeof( uint8_t ) );
        packet->WriteBytes( (void*)data, msgSize );
    }

    m_owningSession->SendPacket( packet );
    

}