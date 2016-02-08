//=================================================================================
// NetworkPacketQueue.cpp
// Author: Tyler George
// Date  : January 27, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Networking/NetworkPacketQueue.hpp"
#include "Engine/Networking/NetworkPacket.hpp"

////===========================================================================================
///===========================================================================================
// NetworkPacketQueue class
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Outgoing
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkPacketQueue::EnqueueWrite( NetworkPacket* packet )
{
    m_outgoing.Enqueue( packet );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkPacketQueue::EnqueueWrite( unsigned char* buffer, size_t len, sockaddr* addr, size_t addrlen )
{
    NetworkPacket* packet = new NetworkPacket( buffer, len, addr, addrlen );
    packet->SetLength( len );
    EnqueueWrite( packet );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkPacket* NetworkPacketQueue::DequeueWrite()
{
    NetworkPacket* packet = nullptr;
    if (m_outgoing.Dequeue( &packet ))
        return packet;

    return nullptr;
}

////===========================================================================================
///===========================================================================================
// Incoming
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkPacketQueue::EnqueueRead( NetworkPacket* packet )
{
    m_incoming.Enqueue( packet );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkPacketQueue::EnqueueRead( unsigned char* buffer, size_t len, sockaddr* addr, size_t addrlen )
{
    NetworkPacket* packet = new NetworkPacket( buffer, len, addr, addrlen );
    packet->SetLength( len );
    EnqueueRead( packet );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkPacket* NetworkPacketQueue::DequeueRead()
{
    NetworkPacket* packet = nullptr;
    if (m_incoming.Dequeue( &packet ))
        return packet;
    return nullptr;
}