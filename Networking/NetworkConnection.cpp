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
#include "Engine/Utilities/Time.hpp"
#include <algorithm>

////===========================================================================================
///===========================================================================================
// ReliableTracker class
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
ReliableTracker::ReliableTracker( uint16_t ack, size_t count )
{
    m_timeCreated = GetCurrentSeconds();
    m_packetAck = ack;
    m_reliableIDs.reserve( count );
}



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
    , m_timeLastReceivedPacket( GetCurrentSeconds() )
    , m_timeLastSentHeartbeat( GetCurrentSeconds() )
    , m_state( eConnectionState_Connected )
    , m_connectionIndex( CONNECTION_INVALID_INDEX )
    , m_uniqueID( "" )
    , m_oldestUsedReliableID( CONNECTION_MAX_RELIABLE_ID )
    , m_nextReliableID( 0 )
    , m_nextIncomingOrderID( 0 )
    , m_nextOutgoingOrderID( 0 )
    , m_numTestInOrderMsgsToSend( 0 )
    , m_nextInOrderTestValue( 0 )
        
{
//     m_uniqueID = NetworkSystem::GetLocalHostName();
}
    
///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkConnection::~NetworkConnection()
{

}

////===========================================================================================
///===========================================================================================
// Accessors/Queries
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkConnection::IsMe() const
{
    return m_owningSession->GetMe() == this;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkConnection::ShouldSendHeartbeat()
{
    if (IsMe())
    {
        return false;
    }

    double currentTime = GetCurrentSeconds();
    double age = currentTime - this->m_timeLastSentHeartbeat;
    if (age > CONNECTION_HEARTBEAT_TIME)
    {
        return true;
    }
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkConnection::CanUseReliableID( uint16_t reliableID )
{
    if (reliableID < m_oldestUsedReliableID)
        reliableID += CONNECTION_MAX_RELIABLE_ID;

    uint16_t distance = reliableID - m_oldestUsedReliableID;
    return distance < CONNECTION_RELIABLE_WINDOW_SIZE;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkConnection::HasRecievedOrderID( uint16_t orderID )
{
    std::map< uint16_t, NetworkMessage >::iterator msgIter = m_unprocessedInOrderMsgs.find( orderID );

    if (msgIter != m_unprocessedInOrderMsgs.end())
        return true;
    else
        return false;
}


////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::AddMessage( const NetworkMessage& msg )
{
    NetworkMessage* newMsg = new NetworkMessage( msg );

    NetworkMessageDefinition* def = newMsg->GetDefinition();

    if (def->IsInOrder())
    {
        newMsg->SetOrderID( m_nextOutgoingOrderID );
        IncrementOrderID( m_nextOutgoingOrderID );
    }

    if (def->IsReliable())
        m_unsentReliables.push( newMsg );
    else
        m_unsentUnreliables.push_back( newMsg );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::CheckForDisconnect()
{
    if (IsMe())
    {
        return;
    }

    double currentTime = GetCurrentSeconds();
    double age = currentTime - m_timeLastReceivedPacket;
    if (age > CONNECTION_TIMEOUT)
    {
        SetState( eConnectionState_Disconnected );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::SendHeartbeat()
{
    // However you send messages to your connection
    this->AddMessage( NetworkMessage( MSG_TYPE_HEARTBEAT ) );

    // Update clock
    m_timeLastSentHeartbeat = GetCurrentSeconds();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::NetworkUpdate()
{
    // Heartbeat
    this->CheckForDisconnect();
    if (!this->IsConnected())
        return;

    if (ShouldSendHeartbeat())
        SendHeartbeat();

    // In-Order testing
    if (m_numTestInOrderMsgsToSend != 0)
    {
        if (m_nextInOrderTestValue == m_numTestInOrderMsgsToSend)
        {
            m_numTestInOrderMsgsToSend = 0;
            m_nextInOrderTestValue = 0;
        }

        else
        {
            NetworkMessage msg( MSG_TYPE_IN_ORDER_TEST );
            msg.WriteBytes( &m_nextInOrderTestValue, sizeof( m_nextInOrderTestValue ) );
            AddMessage( msg );
            ++m_nextInOrderTestValue;
        }

    }
    
    CleanupTrackers();
    FlushMessages();
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::FlushMessages()
{
    // setup packet
    NetworkPacket* packet = nullptr; 

    NetworkMessageQueue reliablesSentThisFrame;

    // resend old reliables
    while (m_sentRelieables.size() > 0)
    {
        if (!packet)
            packet = CreateNewPacket();

        NetworkMessage* msg = m_sentRelieables.front();

        // if this got back to me, it will mark free
        if (m_sentReliableIDs.IsSet( msg->GetReliableID() ) == false)
        {
            m_sentRelieables.pop();
            delete msg;
        }
        else
        {
            // resend if it has passed resend age
            double age = GetCurrentSeconds() - msg->GetLastTimeSent();
            if (age >= MESSAGE_RESEND_TIME)
            {
                if (packet->AddMessage( msg ))
                {
                    m_sentRelieables.pop();
                    reliablesSentThisFrame.push( msg );
                }
                else
                    break;
            }
            else
                break;
        }
    }

    // send unsent reliables
    while (m_unsentReliables.size() > 0)
    {
        if (!packet)
            packet = CreateNewPacket();

        NetworkMessage* msg = m_unsentReliables.front();
        uint16_t reliableID = m_nextReliableID;

        // is the next reliable ID available?
        // if not stop sending new traffic
        if (!CanUseReliableID( reliableID ))
            break;

        msg->SetReliableID( reliableID );
        if (packet->AddMessage( msg ))
        {
            IncrementReliableID( m_nextReliableID );
            m_unsentReliables.pop();
            reliablesSentThisFrame.push( msg );
            m_sentReliableIDs.Set( msg->GetReliableID() );
        }
        else
            break; // no more room in packet
    }

    // send acks
    SendAllReceivedAcks( packet );


    // send unreliables
    for (NetworkMessages::iterator msgIter = m_unsentUnreliables.begin(); msgIter != m_unsentUnreliables.end(); )
    {
        if (!packet)
            packet = CreateNewPacket();

        NetworkMessage* msg = *msgIter;
        packet->AddMessage( msg ); // don't care if unreliable fails to be added to a packet

        // cleanup
        delete msg;
        msgIter = m_unsentUnreliables.erase( msgIter );
    }

    if( packet )
        m_owningSession->SendPacket( packet );

    m_lastSendTime = (float)Clock::GetMasterClock()->GetCurrentSeconds();


    // Track which reliables were sent with this packet
    if (reliablesSentThisFrame.size() > 0)
    {
        ReliableTracker* tracker = new ReliableTracker( m_nextAckID, reliablesSentThisFrame.size() );
        while (reliablesSentThisFrame.size() > 0)
        {
            NetworkMessage* msg = reliablesSentThisFrame.front();
            reliablesSentThisFrame.pop();
            tracker->m_reliableIDs.push_back( msg->GetReliableID() );
            msg->SetLastTimeSent( GetCurrentSeconds() );
            m_sentRelieables.push( msg );
        }
        m_trackers.push_back( tracker );
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::IncrementAck( uint16_t& ack )
{
    ++ack;
    if (ack == INVALID_ACK)
        ++ack;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::IncrementReliableID( uint16_t& reliableID )
{
    ++reliableID;
    if (reliableID == CONNECTION_MAX_RELIABLE_ID)
        ++reliableID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::IncrementOrderID( uint16_t& orderID )
{
    ++orderID;
    if (orderID == MESSAGE_INVALID_ORDER_ID)
        ++orderID;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::CleanupTrackers()
{
    for (std::vector< ReliableTracker* >::iterator trackerIter = m_trackers.begin(); trackerIter != m_trackers.end(); )
    {
        ReliableTracker* tracker = *trackerIter;

        if (tracker)
        {
            double age = GetCurrentSeconds() - tracker->m_timeCreated;
            if (age >= CONNECTION_TRACKER_MAX_AGE)
            {
                trackerIter = m_trackers.erase( trackerIter );
                delete tracker;
            }
            else
                ++trackerIter;
        }
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
ReliableTracker* NetworkConnection::FindAndRemoveTracker( uint16_t ack )
{
    for (std::vector< ReliableTracker* >::iterator trackerIter = m_trackers.begin(); trackerIter != m_trackers.end(); ++trackerIter )
    {
        ReliableTracker* tracker = *trackerIter;
        if (tracker->m_packetAck == ack)
        {
            m_trackers.erase( trackerIter );
            return tracker;
        }
    }

    return nullptr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::SendAllReceivedAcks( NetworkPacket* packet )
{
    if (m_trackedAcks.size() == 0)
        return;

    uint8_t receivedAckCount = max( 0xFF, (uint8_t)m_trackedAcks.size() );

    NetworkMessage* ackMessage = new NetworkMessage( MSG_TYPE_ACK );
    ackMessage->WriteBytes( (void*)&receivedAckCount, sizeof( receivedAckCount ) );

    for (std::vector< uint16_t >::iterator ackIter = m_trackedAcks.begin(); ackIter != m_trackedAcks.end(); ++ackIter)
    {
        uint16_t ack = *ackIter;
        ackMessage->WriteBytes( (void*)&ack, sizeof( ack ) );
    }

    m_trackedAcks.clear();

    if (!packet)
        packet = CreateNewPacket();

    packet->AddMessage( ackMessage );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------    
bool NetworkConnection::GetNextInOrderMessageFromCache( NetworkMessage& out_msg )
{
    std::map< uint16_t, NetworkMessage >::iterator msgIter = m_unprocessedInOrderMsgs.find( m_nextIncomingOrderID );

    NetworkMessage msg = msgIter->second;

    if (msgIter != m_unprocessedInOrderMsgs.end())
    {
        out_msg = msg;
        return true;
    }
    else
        return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkConnection::AddInOrderMessageToCache( const NetworkMessage& msg )
{
    NetworkMessage newMsg( msg );

    uint16_t orderID = newMsg.GetOrderID();

    if (HasRecievedOrderID( orderID ))
        return false;

    else
    {
        m_unprocessedInOrderMsgs.insert( std::pair< uint16_t, NetworkMessage >( orderID, newMsg ) );
        return true;
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::RemoveInOrderMessageFromCache( uint16_t orderID )
{
    std::map< uint16_t, NetworkMessage >::iterator msgIter = m_unprocessedInOrderMsgs.find( orderID );

    if (msgIter != m_unprocessedInOrderMsgs.end())
        m_unprocessedInOrderMsgs.erase( msgIter );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkConnection::SetNumTestInOrderMsgsToSend( uint32_t numMsgs )
{
    m_numTestInOrderMsgsToSend = numMsgs;
    m_nextInOrderTestValue = 0;
}

////===========================================================================================
///===========================================================================================
// Private functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkPacket* NetworkConnection::CreateNewPacket()
{
    NetworkPacket* packet = new NetworkPacket( &m_address, m_nextAckID, m_owningSession->GetMe()->GetConnectionIndex() );
    IncrementAck( m_nextAckID );

    return packet;
}