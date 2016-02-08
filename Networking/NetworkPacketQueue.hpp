//=================================================================================
// NetworkPacketQueue.hpp
// Author: Tyler George
// Date  : January 25, 2016
//=================================================================================
#pragma once

#ifndef __included_NetworkPacketQueue__
#define __included_NetworkPacketQueue__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine\Multi-Threading\ThreadSafeQueue.hpp"

class NetworkPacket;
struct sockaddr;

///---------------------------------------------------------------------------------
/// NetworkPacketQueue class
///---------------------------------------------------------------------------------
class NetworkPacketQueue
{
public:
    ///---------------------------------------------------------------------------------
    /// Outgoing
    ///---------------------------------------------------------------------------------
    void EnqueueWrite( NetworkPacket* packet );
    void EnqueueWrite( unsigned char* buffer, size_t len, sockaddr* addr, size_t addrlen );
    NetworkPacket* DequeueWrite();

    ///---------------------------------------------------------------------------------
    /// Incoming
    ///---------------------------------------------------------------------------------
    void EnqueueRead( NetworkPacket* packet );
    void EnqueueRead( unsigned char* buffer, size_t len, sockaddr* addr, size_t addrlen );
    NetworkPacket* DequeueRead();

private:
    ///---------------------------------------------------------------------------------
    /// private member variables
    ///---------------------------------------------------------------------------------
    ThreadSafeQueue< NetworkPacket* > m_incoming;
    ThreadSafeQueue< NetworkPacket* > m_outgoing;
};

#endif
