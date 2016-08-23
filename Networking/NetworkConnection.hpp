//=================================================================================
// NetworkConnection.hpp
// Author: Tyler George
// Date  : February 3, 2016
//=================================================================================
#pragma once

#ifndef __included_NetworkConnection__
#define __included_NetworkConnection__


///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <stdint.h>
#include <vector>
#include "Engine/Networking/NetworkAddress.hpp"
#include "Engine/Networking/NetworkMessage.hpp"
#include "Engine/Utilities/BitArray.hpp"
class NetworkSession;

///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
double const CONNECTION_TIMEOUT = 30.0;
double const CONNECTION_HEARTBEAT_TIME = 2.5;
int const CONNECTION_RELIABLE_WINDOW_SIZE = 4096;
int const CONNECTION_MAX_RELIABLE_ID = 0xFFFF;
double const CONNECTION_TRACKER_MAX_AGE = 2.5;
uint8_t const CONNECTION_INVALID_INDEX = 0xFF;

///---------------------------------------------------------------------------------
/// Enums
///---------------------------------------------------------------------------------
enum eConnectionState
{
    eConnectionState_Connected, // starts in this state
    eConnectionState_Disconnected,
};

///---------------------------------------------------------------------------------
/// Reliable Tracker class
///---------------------------------------------------------------------------------
class ReliableTracker
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ReliableTracker( uint16_t ack, size_t count );

    ///---------------------------------------------------------------------------------
    /// Public member variables
    ///---------------------------------------------------------------------------------
    double m_timeCreated; // when this tracker was created
    uint16_t m_packetAck; // Packet this is for
    std::vector<uint16_t> m_reliableIDs; // Reliable IDs sent with this ack

};

///---------------------------------------------------------------------------------
/// NetworkConnection class
///---------------------------------------------------------------------------------
class NetworkConnection
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    NetworkConnection( const NetworkAddress& address, NetworkSession* owningSession );
    ~NetworkConnection();

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    NetworkAddress GetAddress() { return m_address; }
    bool IsMe() const;
    bool ShouldSendHeartbeat();
    bool IsConnected() const { return m_state == eConnectionState_Connected; }
    bool CanUseReliableID( uint16_t reliableID );
    TBitArray<CONNECTION_MAX_RELIABLE_ID> GetReceivedReliableIDs() { return m_receivedReliableIDs; }
    TBitArray<CONNECTION_MAX_RELIABLE_ID> GetSentReliableIDs() { return m_sentReliableIDs; }
    std::vector< uint16_t >& GetTrackedAcks() { return m_trackedAcks; }
    uint8_t GetConnectionIndex() const { return m_connectionIndex; }
    std::string GetUniqueID() const { return m_uniqueID; }

    // In-Order
    bool HasRecievedOrderID( uint16_t orderID );
    bool HasRecievedNextOrderID() { return HasRecievedOrderID( m_nextIncomingOrderID ); }
    uint16_t GetNextIncomingOrderID() { return m_nextIncomingOrderID; }

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void AddMessage( const NetworkMessage& msg );
    void CheckForDisconnect();
    void SetState( eConnectionState state ) { m_state = state; }
    void SetTimeLastRecievedPacket( double time ) { m_timeLastReceivedPacket = time; }
    void SendHeartbeat();
    void NetworkUpdate();
    void FlushMessages();

    void IncrementAck( uint16_t& ack );
    void IncrementReliableID( uint16_t& reliableID );
    void CleanupTrackers();
    ReliableTracker* FindAndRemoveTracker( uint16_t ack );
    void SendAllReceivedAcks( NetworkPacket* packet );
    void SetConnectionIndex( uint8_t idx ) { m_connectionIndex = idx; }
    void SetUniqueID( const std::string& uniqueID ) { m_uniqueID = uniqueID; }

    // In-Order
    void IncrementOrderID( uint16_t& orderID );
    void IncrementNextIncomingOrderID() { IncrementOrderID( m_nextIncomingOrderID ); }
    bool GetNextInOrderMessageFromCache( NetworkMessage& out_msg );
    bool AddInOrderMessageToCache( const NetworkMessage& msg );
    void RemoveInOrderMessageFromCache( uint16_t orderID );

    void SetNumTestInOrderMsgsToSend( uint32_t numMsgs );



private:
    ///---------------------------------------------------------------------------------
    /// Private functions
    ///---------------------------------------------------------------------------------
    NetworkPacket* CreateNewPacket();

    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    std::string m_uniqueID;
    uint8_t m_connectionIndex;

    NetworkSession* m_owningSession;
    NetworkAddress m_address;
    eConnectionState m_state;

    double m_lastSendTime;
    double m_timeLastReceivedPacket;
    double m_timeLastSentHeartbeat;

    uint16_t m_nextAckID;
    uint16_t m_nextReliableID;
    uint16_t m_oldestUsedReliableID;
    TBitArray<CONNECTION_MAX_RELIABLE_ID> m_sentReliableIDs;
    TBitArray<CONNECTION_MAX_RELIABLE_ID> m_receivedReliableIDs;

    NetworkMessageQueue m_unsentReliables;
    NetworkMessageQueue m_sentRelieables;
    NetworkMessages m_unsentUnreliables;

    std::vector< ReliableTracker* > m_trackers;
    std::vector< uint16_t > m_trackedAcks;

    // In-Order
    std::map< uint16_t, NetworkMessage > m_unprocessedInOrderMsgs;
    uint16_t m_nextOutgoingOrderID;
    uint16_t m_nextIncomingOrderID;

    // Testing
    uint32_t m_numTestInOrderMsgsToSend;
    uint32_t m_nextInOrderTestValue;
};

#endif