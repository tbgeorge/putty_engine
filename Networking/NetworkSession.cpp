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
    , m_hostConnection( nullptr )
    , m_me( nullptr )
    , m_state( NETWORK_DISCONNECTED )
    , m_isJoinable( false )
    , m_maxNumConnections( 0 )
    , m_nextConnectionIndex( 1 )
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
        if (conn == m_hostConnection)
            m_hostConnection = nullptr;
        delete conn;
        connIter = m_connections.erase( connIter );
    }

    if( m_hostConnection )
        delete m_hostConnection;
    if (m_me)
        //delete m_me;

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
    if (!netSys)
        return false;

    UDPSocket* sock = netSys->CreateUDPSocket( m_packetQueue, port );
    if (!sock)
        return false;

    m_sockets.push_back( sock );

    m_me = new NetworkConnection( sock->GetAddress(), this );
    m_me->SetConnectionIndex( CONNECTION_INVALID_INDEX );
    m_me->SetUniqueID( NetworkSystem::GetLocalHostName() );

    SetState( NETWORK_DISCONNECTED );
    m_isJoinable = false;
    m_maxNumConnections = 0; 

    return true;
     
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkSession::HostSession( short port )
{
    UNUSED( port );

    // session has not been created
    if (!m_me)
        return false;
    m_me->SetConnectionIndex( 0 );

    // set host connection and add to connection list
    m_hostConnection = m_me;
//     m_connections.push_back( m_hostConnection );

    SetState( NETWORK_HOSTING );
    m_isJoinable = true;
    m_maxNumConnections = 8;

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
bool NetworkSession::JoinSession( NetworkAddress& addr, const std::string& uniqueID )
{
    if (m_state != NETWORK_DISCONNECTED)
        return false;

    m_hostConnection = new NetworkConnection( addr, this );
    m_hostConnection->SetConnectionIndex( 0 );
    m_connections.push_back( m_hostConnection );

    NetworkMessage msg( MSG_TYPE_JOIN_REQUEST );
    msg.WriteString( uniqueID.c_str() );

    m_hostConnection->AddMessage( msg );
//     SendDirectMessage( msg, &addr );
    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::DisconnectFromHost()
{
    if (m_hostConnection && m_state == NETWORK_CONNECTED )
    {
        m_hostConnection->FlushMessages();

        delete m_hostConnection;
        m_hostConnection = nullptr;
        m_connections.clear();

        SetState( NETWORK_DISCONNECTED );
        SetMyConnectionIndex( CONNECTION_INVALID_INDEX );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkConnection* NetworkSession::AddConnection( const NetworkAddress& addr, const std::string& uniqueID, AddConnectionResult& out_result )
{
    NetworkConnection* conn = FindConnection( addr );
    out_result = ACR_DENIED;

    if (conn)
        return conn;

    // I'm hosting, I'm joinable, not full, and the unique ID given is not taken
    if (m_state == NETWORK_HOSTING && IsJoinable() && !IsFull() && !IsUniqueIDTaken( uniqueID ))
    {
        out_result = ACR_ACCEPTED;
        NetworkConnection* newConn = new NetworkConnection( addr, this );
        m_connections.push_back( newConn );
        newConn->SetConnectionIndex( m_nextConnectionIndex++ );
        newConn->SetUniqueID( uniqueID );
        return newConn;
    }
    else if (IsFull())
        out_result = ACR_FULL;
    else if (IsUniqueIDTaken( uniqueID ))
        out_result = ACR_ID_TAKEN;

    return nullptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkSession::RemoveConnection( const NetworkAddress& addr )
{
    for (std::vector< NetworkConnection* >::iterator connIter = m_connections.begin(); connIter != m_connections.end(); ++connIter)
    {
        NetworkConnection* conn = *connIter;
        if (conn->GetAddress().Equals( addr ))
        {
            delete conn;
            m_connections.erase( connIter );
            return true;
        }
    }
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::ProcessPacket( NetworkPacket* packet )
{
    uint8_t connectionID;
    packet->ReadBytes( &connectionID, sizeof( connectionID ) );

    uint16_t packetAck;
    packet->ReadBytes( &packetAck, sizeof( packetAck ) );

    uint8_t messageCount;
    packet->ReadBytes( &messageCount, sizeof( messageCount ) );

    // construct sender
    NetworkConnection* conn = FindConnection( *(packet->GetAddress()) );
    NetworkSender sender( *(packet->GetAddress()), this, conn );
    if (conn)
        conn->SetTimeLastRecievedPacket( GetCurrentSeconds() );

    NetworkMessage msg;
    bool trackAck = false;
    while (msg.InitFromPacket( packet ))
    {
        if (CanProcessMessage( sender, msg ))
            ProcessMessage( sender, msg );

        // track acks for reliable messages
        trackAck = trackAck || ((msg.GetDefinition()->IsReliable() && sender.m_connection != nullptr));
    }

    if (trackAck && (packetAck != INVALID_ACK))
    {
        if (sender.m_connection->GetTrackedAcks().size() < 0xFF)
            sender.m_connection->GetTrackedAcks().push_back( packetAck );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::ProcessMessage( NetworkSender& sender, NetworkMessage& msg )
{
    // In-Order
    if (msg.GetDefinition()->IsInOrder())
    {
        uint16_t expectedID = sender.m_connection->GetNextIncomingOrderID();

        if (msg.GetOrderID() != expectedID)
        {
            sender.m_connection->AddInOrderMessageToCache( msg );
            return;
        }
        else
        {
            sender.m_connection->RemoveInOrderMessageFromCache( expectedID );
            sender.m_connection->IncrementNextIncomingOrderID();
        }
    }

    msg.GetDefinition()->GetCallback()(sender, msg);

    NetworkConnection *conn = sender.m_connection;
    if (conn == nullptr)
    {
        conn = sender.m_session->FindConnection( sender.m_addr );
        sender.m_connection = conn;
    }

    if ((conn != nullptr) && msg.GetDefinition()->IsReliable())
    {
        conn->GetReceivedReliableIDs().Set( msg.GetReliableID() );

        // Unset the reliable id 
        uint16_t reliableID = msg.GetReliableID();
        uint16_t offset = 2 * CONNECTION_RELIABLE_WINDOW_SIZE;
        if (offset > reliableID)
            reliableID = CONNECTION_MAX_RELIABLE_ID - (offset - reliableID);
        else
            reliableID = reliableID - offset;

        conn->GetReceivedReliableIDs().Unset( reliableID );
    }

    // process cached messages
    if (msg.GetDefinition()->IsInOrder())
    {
        if (sender.m_connection->HasRecievedNextOrderID())
        {
            bool success = sender.m_connection->GetNextInOrderMessageFromCache( msg );
            if (success)
                ProcessMessage( sender, msg );
        }
    }
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
void NetworkSession::AddMessage( const NetworkMessage& msg )
{
    for (std::vector< NetworkConnection* >::iterator connIter = m_connections.begin(); connIter != m_connections.end(); ++connIter)
    {
        NetworkConnection* conn = *connIter;
        conn->AddMessage( NetworkMessage( msg ) );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::SendDirectMessage( NetworkMessage* msg, NetworkAddress* addr )
{
    NetworkPacket* packet = new NetworkPacket( addr, 0 );
    packet->SetAddress( new NetworkAddress( *addr ) );
    packet->AddMessage( msg );

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
    float currentTime = (float) Clock::GetMasterClock()->GetCurrentSeconds();
    
    // TODO: check if host has disconnected

    float timeElapsed = currentTime - m_lastTimeSent;
    if (timeElapsed < m_updateRate)
        return;

    NetworkPacket* currentPacket = m_packetQueue->DequeueRead();
    while (currentPacket != nullptr)
    {
        NetworkMessages msgs;
        bool valid = NetworkPacket::ValidatePacket( currentPacket, msgs );
        if( valid )
            ProcessPacket( currentPacket );


        // get next packet
        delete currentPacket;
        currentPacket = m_packetQueue->DequeueRead();
    }

    for (std::vector< NetworkConnection* >::iterator connIter = m_connections.begin(); connIter != m_connections.end(); connIter++)
    {
        NetworkConnection* conn = *connIter;
        conn->NetworkUpdate();
    }

    m_lastTimeSent = (float) Clock::GetMasterClock()->GetCurrentSeconds();


}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSession::SetMyConnectionIndex( uint8_t connectionIdx )
{
    m_me->SetConnectionIndex( connectionIdx );
    if( connectionIdx != CONNECTION_INVALID_INDEX )
        SetState( NETWORK_CONNECTED );
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

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkSession::CanProcessMessage( NetworkSender& sender, NetworkMessage& msg )
{
    // Message Option: CONNECTIONLESS
    if (!msg.GetDefinition()->IsConnectionless() && sender.m_connection == nullptr)
        false;

    // Message Option: RELIABLE
    if (msg.GetDefinition()->IsReliable())
    {
        NetworkConnection* conn = sender.m_connection;
        if (conn != nullptr)
        {
            if (conn->GetReceivedReliableIDs().IsSet( msg.GetReliableID() ) == true)
                return false;
        }
    }

    // Message Option: IN-ORDER
    if (msg.GetDefinition()->IsInOrder())
        return true;

    return true;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkSession::IsUniqueIDTaken(const std::string& id) const
{
    for (std::vector< NetworkConnection* >::const_iterator connIter = m_connections.begin(); connIter != m_connections.end(); ++connIter)
    {
        NetworkConnection* conn = *connIter;
        if (conn->GetUniqueID() == id)
            return true;
    }

    return false;
}