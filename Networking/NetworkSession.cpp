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
//     UNUSED( packet );
    m_packetQueue->EnqueueWrite( packet );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::SendMessage( NetworkMessage& msg )
{
    UNUSED( msg );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::Update()
{
    NetworkPacket* currentPacket = m_packetQueue->DequeueRead();
    while (currentPacket != nullptr)
    {
        NetworkMessages msgs;
        bool valid = NetworkPacket::ValidatePacket( currentPacket, msgs );

        NetworkConnection* conn = FindConnection( *currentPacket->GetAddress() );
        // connection exists
        if (conn != nullptr)
        {
            if (valid)
            {
                for (NetworkMessages::iterator msgIter = msgs.begin(); msgIter != msgs.end(); ++msgIter)
                {
                    NetworkMessage* msg = *msgIter;
                    NetworkMessage::ProcessMessage( conn, msg );
                }
            }
        }
        // else connection does not exist - create one?


        // get next packet
        currentPacket = m_packetQueue->DequeueRead();
    }

    for (std::vector< NetworkConnection* >::iterator connIter = m_connections.begin(); connIter != m_connections.end(); connIter++)
    {
        NetworkConnection* conn = *connIter;
        conn->Update();
    }
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
