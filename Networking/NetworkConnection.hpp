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
class NetworkSession;

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

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void SendNetworkMessage( NetworkMessage* msg );
    void Update();


private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    uint16_t m_nextAckID;
    NetworkMessages m_outgoingMessages;
    float m_lastSendTime;

    NetworkAddress m_address;
    NetworkSession* m_owningSession;

};

#endif