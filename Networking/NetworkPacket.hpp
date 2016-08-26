//=================================================================================
// NetworkPacket.hpp
// Author: Tyler George
// Date  : January 25, 2016
//=================================================================================
#pragma once

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <vector>
#include "Engine/Networking/NetworkAddress.hpp"
#include "Engine/Utilities/ByteBuffer.hpp"
#include <stdint.h>
class NetworkMessage;

#ifndef __included_NetworkPacket__
#define __included_NetworkPacket__

///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
const size_t PACKET_MTU = 1400;
const uint16_t INVALID_ACK = 0xFFFF;

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class NetworkPacket : public ByteBuffer
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    NetworkPacket();
    NetworkPacket( NetworkAddress* address, uint16_t ackID, uint8_t connectionIndex = 0xFF );
    NetworkPacket( void* data, size_t dataLen, sockaddr* saddr, size_t addrlen );

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    NetworkAddress* GetAddress() const { return m_address; }
    const unsigned char* GetBuffer() const { return m_buf; }

    static bool ValidatePacket( NetworkPacket* packet, std::vector< NetworkMessage* >& out_messages );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    void SetAddress( NetworkAddress* addr ) { m_address = addr; }
    bool AddMessage( NetworkMessage* msg );

private:
    ///---------------------------------------------------------------------------------
    /// private member variables
    ///---------------------------------------------------------------------------------
    unsigned char m_buf[PACKET_MTU];
    NetworkAddress* m_address; // to or from, depending on usage
    uint8_t m_connectionID;
    uint16_t m_packetAck;
    uint8_t m_numMessages;

};


#endif
