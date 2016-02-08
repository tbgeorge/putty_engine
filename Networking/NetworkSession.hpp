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
    void Listen( bool listening );

    NetworkConnection* AddConnection( const NetworkAddress& addr );

    void SendPacket( NetworkPacket* packet );
    void SendNetworkMessage( const NetworkMessage& msg );
    void SendDirectMessage( NetworkMessage* msg, NetworkAddress* addr );
    void SetSendFrequency( float hz );
    void Update();

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    NetworkConnection* FindConnection( const NetworkAddress& addr );

public:
    ///---------------------------------------------------------------------------------
    /// private member variables
    ///---------------------------------------------------------------------------------
    NetworkPacketQueue* m_packetQueue;
    std::vector< UDPSocket* > m_sockets;
    std::vector< NetworkConnection* > m_connections;
    NetworkConnection* m_hostConnection;
    bool m_listening;

    float m_updateRate;
    float m_lastTimeSent;
};

#endif