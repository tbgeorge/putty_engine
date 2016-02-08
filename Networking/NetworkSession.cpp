//=================================================================================
// NetworkSession.cpp
// Author: Tyler George
// Date  : January 27, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Networking/NetworkSession.hpp"
#include "Engine/Networking/NetworkPacketQueue.hpp"
#include "../Utilities/Time.hpp"
#include "Engine/Low-Level/Memory.hpp"

////===========================================================================================
///===========================================================================================
// NetworkSession class
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
NetworkSession::NetworkSession()
    : m_lastTimeSent( 0.0f )
    , m_updateRate( 1.0f )
{
    m_packetQueue = new NetworkPacketQueue();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkSession::~NetworkSession()
{
    // close and delete all sockets
    for (std::vector< UDPSocket* >::iterator sockiter = m_sockets.begin(); sockiter != m_sockets.end();)
    {
        UDPSocket* sock = *sockiter;
        sock->Join();

        delete sock;
        sockiter = m_sockets.erase( sockiter );
    }

    for (std::vector< NetworkConnection* >::iterator connIter = m_connections.begin(); connIter != m_connections.end(); )
    {
        NetworkConnection* conn = *connIter;
        delete conn;
        connIter = m_connections.erase( connIter );
    }

    delete m_hostConnection;

    // delete packet queue
    delete m_packetQueue;
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------    
bool NetworkSession::Start( short port )
{
    NetworkSystem* netSys = NetworkSystem::GetInstance();

    UDPSocket* sock = netSys->CreateUDPSocket( m_packetQueue, port );
    m_sockets.push_back( sock );

    m_hostConnection = new NetworkConnection( sock->GetAddress(), this );

    return true;
     
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::Listen( bool listening )
{
    m_listening = listening;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkConnection* NetworkSession::AddConnection( const NetworkAddress& addr )
{
    // TODO: add check to see if connection already exists
    NetworkConnection* newConn = new NetworkConnection( addr, this );
    m_connections.push_back( newConn );
    return newConn;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::SendPacket( NetworkPacket* packet )
{
    m_packetQueue->EnqueueWrite( packet );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::SendNetworkMessage( const NetworkMessage& msg )
{
    for (std::vector< NetworkConnection* >::iterator connIter = m_connections.begin(); connIter != m_connections.end(); ++connIter)
    {
        NetworkConnection* conn = *connIter;
        conn->SendNetworkMessage( new NetworkMessage( msg ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::SendDirectMessage( NetworkMessage* msg, NetworkAddress* addr )
{
    NetworkPacket* packet = new NetworkPacket();
    packet->SetAddress( addr );

    uint16_t ackID = 0;
    packet->WriteBytes( (void*)&ackID, sizeof( uint16_t ) );

    uint8_t numMsgs = 1;
    packet->WriteBytes( (void*)&numMsgs, sizeof( uint8_t ) );
    packet->WriteBytes( (void*)msg->m_buf, msg->GetLength() );

    SendPacket( packet );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::SetSendFrequency( float hz )
{
    m_updateRate = 1.0f / hz;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::Update()
{
    float currentTime = Clock::GetMasterClock()->GetCurrentSeconds();

    float timeElapsed = currentTime - m_lastTimeSent;
    if (timeElapsed < m_updateRate)
        return;

    NetworkPacket* currentPacket = m_packetQueue->DequeueRead();
    while (currentPacket != nullptr)
    {
        NetworkMessages msgs;
        bool valid = NetworkPacket::ValidatePacket( currentPacket, msgs );

        NetworkConnection* conn = FindConnection( *currentPacket->GetAddress() );

        NetworkSender sender( *currentPacket->GetAddress(), this, conn );

        if (valid)
        {
            for (NetworkMessages::iterator msgIter = msgs.begin(); msgIter != msgs.end(); ++msgIter)
            {
                NetworkMessage* msg = *msgIter;
                NetworkMessage::ProcessMessage( sender, msg );
            }
        }


        // get next packet
        delete currentPacket;
        currentPacket = m_packetQueue->DequeueRead();
    }

    for (std::vector< NetworkConnection* >::iterator connIter = m_connections.begin(); connIter != m_connections.end(); connIter++)
    {
        NetworkConnection* conn = *connIter;
        conn->Update();
    }

    m_lastTimeSent = Clock::GetMasterClock()->GetCurrentSeconds();


}

////===========================================================================================
///===========================================================================================
//  Accessors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkConnection* NetworkSession::FindConnection( const NetworkAddress& addr )
{
    for (std::vector< NetworkConnection* >::iterator connectionIter = m_connections.begin(); connectionIter != m_connections.end(); ++connectionIter)
    {
        NetworkConnection* conn = *connectionIter;

        if (conn->GetAddress().Equals( addr ))
            return conn;
    }

    return nullptr;
}
