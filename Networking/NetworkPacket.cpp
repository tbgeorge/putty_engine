//=================================================================================
// NetworkPacket.cpp
// Author: Tyler George
// Date  : January 27, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Networking/NetworkPacket.hpp"
#include <stdint.h>
#include "NetworkMessage.hpp"
#include <vector>

////===========================================================================================
///===========================================================================================
// NetworkPacket class
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
NetworkPacket::NetworkPacket()
{
    ByteBuffer::Startup( m_buf, PACKET_MTU );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkPacket::NetworkPacket( void* data, size_t dataLen, sockaddr* saddr, size_t addrlen )
{
    ByteBuffer::Startup( m_buf, PACKET_MTU );
    WriteBytes( data, dataLen );
    m_address->Startup( saddr, addrlen );
}

////===========================================================================================
///===========================================================================================
// Accessors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkPacket::ValidatePacket( NetworkPacket* packet, std::vector< NetworkMessage* >& out_messages )
{
    size_t packetSize = 0;

    uint16_t packetAck;
    packet->ReadBytes( &packetAck, sizeof( uint16_t ) );
    packetSize += sizeof( uint16_t );

    uint8_t messageCount;
    packet->ReadBytes( &messageCount, sizeof( uint8_t ) );
    packetSize += sizeof( uint8_t );

    for (int msgNum = 0; msgNum < messageCount; ++msgNum)
    {
        NetworkMessage* msg = new NetworkMessage( *packet );
        if (msg->GetDefinition() == nullptr)
        {
            // msg definition does not exist
            return false;
        }
        out_messages.push_back( msg );
        packetSize += msg->GetLength();
    }

    if (packetSize > PACKET_MTU)
    {
        // packet is too big
        return false;
    }

    // if all tests have passed
    return true;



}