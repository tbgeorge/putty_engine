//=================================================================================
// NetworkSession.hpp
// Author: Tyler George
// Date  : January 25, 2016
//=================================================================================
#pragma once

#ifndef __included_NetworkSession__
#define __included_NetworkSession__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine/Networking/NetworkSystem.hpp"
#include "Engine/Networking/NetworkAddress.hpp"
#include "Engine/Networking/NetworkConnection.hpp"
#include <vector>

class NetworkPacket;

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------

///---------------------------------------------------------------------------------
/// Enums
///---------------------------------------------------------------------------------
enum NetworkSessionState
{
    NETWORK_DISCONNECTED,
    NETWORK_HOSTING,
    NETWORK_CONNECTED
};

enum AddConnectionResult
{
    ACR_ACCEPTED,
    ACR_FULL,
    ACR_ID_TAKEN,
    ACR_DENIED
};

///---------------------------------------------------------------------------------
/// NetworkSession class
///---------------------------------------------------------------------------------
class NetworkSession
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors
    ///---------------------------------------------------------------------------------
    NetworkSession();
    ~NetworkSession();

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    bool Start( short port );
    bool HostSession( short port );
    void Listen( bool listening );

    bool JoinSession( NetworkAddress& addr, const std::string& uniqueID );
    void DisconnectFromHost();
    NetworkConnection* AddConnection( const NetworkAddress& addr, const std::string& uniqueID, AddConnectionResult& out_result );
    bool RemoveConnection( const NetworkAddress& addr );

    void ProcessPacket( NetworkPacket* packet );
    void ProcessMessage( NetworkSender& sender, NetworkMessage& msg );

    void SendPacket( NetworkPacket* packet );
    void AddMessage( const NetworkMessage& msg );
    void SendDirectMessage( NetworkMessage* msg, NetworkAddress* addr );
    void SetSendFrequency( float hz );
    void Update();
    void SetState( NetworkSessionState state ) { m_state = state; }
    void SetMyConnectionIndex( uint8_t connectionIdx );

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    NetworkConnection* FindConnection( const NetworkAddress& addr );
    NetworkConnection* GetHost() const { return m_hostConnection; }
    NetworkConnection* GetMe() const { return m_me; }
    NetworkPacketQueue* GetPacketQueue() { return m_packetQueue; }
    bool CanProcessMessage( NetworkSender& sender, NetworkMessage& msg );
    NetworkSessionState GetState() const { return m_state; }
    size_t GetMaxConnections() const { return m_maxNumConnections; }
    bool IsJoinable() const { return m_isJoinable; }
    bool IsFull() const { return m_connections.size() >= m_maxNumConnections; }
    bool IsUniqueIDTaken( const std::string& id ) const;

private:
    ///---------------------------------------------------------------------------------
    /// private member variables
    ///---------------------------------------------------------------------------------
    NetworkPacketQueue* m_packetQueue;
    std::vector< UDPSocket* > m_sockets;
    std::vector< NetworkConnection* > m_connections;
    NetworkConnection* m_hostConnection;
    NetworkConnection* m_me;

    bool m_listening;
    float m_updateRate;
    float m_lastTimeSent;

    NetworkSessionState m_state;
    bool m_isJoinable;
    size_t m_maxNumConnections;
    uint8_t m_nextConnectionIndex;
};

#endif