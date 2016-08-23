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
    : m_numMessages( 0 )
{
    ByteBuffer::Startup( m_buf, PACKET_MTU );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkPacket::NetworkPacket( NetworkAddress* address, uint16_t ackID, uint8_t connectionIndex )
    : m_numMessages( 0 )
    , m_packetAck( ackID )
    , m_connectionID( connectionIndex )
{
    ByteBuffer::Startup( m_buf, PACKET_MTU );
    SetAddress( address );    
    WriteBytes( (void*)&m_connectionID, sizeof( m_connectionID ) );
    WriteBytes( (void*)&m_packetAck, sizeof( m_packetAck ) );
    WriteBytes( (void*)&m_numMessages, sizeof( m_numMessages ) );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkPacket::NetworkPacket( void* data, size_t dataLen, sockaddr* saddr, size_t addrlen )
    : m_numMessages( 0 )
{
    ByteBuffer::Startup( m_buf, PACKET_MTU );
    WriteBytes( data, dataLen );
    m_address = new NetworkAddress( saddr, addrlen );
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

    uint8_t connectionID;
    packet->ReadBytes( &connectionID, sizeof( connectionID ) );
    //packetSize += sizeof( connectionID );

    uint16_t packetAck;
    packet->ReadBytes( &packetAck, sizeof( packetAck ) );
    //packetSize += sizeof( packetAck );

    uint8_t messageCount;
    packet->ReadBytes( &messageCount, sizeof( messageCount ) );
    //packetSize += sizeof( messageCount );

    for (int msgNum = 0; msgNum < messageCount; ++msgNum)
    {
        bool successful = false;
        NetworkMessage* msg = new NetworkMessage( packet, successful );
        if (!successful || msg->GetDefinition() == nullptr)
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

    packet->m_readIndex = 0;
    // if all tests have passed
    return true;



}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkPacket::AddMessage( NetworkMessage* msg )
{    
//     if (m_numMessages == 0)
//     {
//         // leave space at the beginning for the number of messages
//         m_writeIndex += sizeof( uint8_t );
//     }
    
    uint16_t totalMsgSize = (uint16_t)(msg->GetHeaderSize() + msg->GetMessageSize());

    if (totalMsgSize > GetRemainingSize())
        return false;
    
    // write size
    WriteBytes( &totalMsgSize, sizeof( uint16_t ) );
    // write id
    uint8_t msgID = msg->GetDefinition()->GetID();
    WriteBytes( &msgID, sizeof( uint8_t ) );
    // write reliable id IF msg is reliable
    if (msg->GetDefinition()->IsReliable())
    {
        uint16_t reliableID = msg->GetReliableID();
        WriteBytes( &reliableID, sizeof( uint16_t ) );
    }
    // write order id IF msg is in-order
    if (msg->GetDefinition()->IsInOrder())
    {
        uint16_t orderID = msg->GetOrderID();
        WriteBytes( &orderID, sizeof( uint16_t ) );
    }


    // write message data
    if (WriteBytes( msg->m_buf, msg->GetMessageSize() ))
    {

        m_numMessages++;

        // manually update the number of messages so the write index is not messed up
        memcpy( m_buf + sizeof( m_connectionID ) + sizeof( m_packetAck ), (void*)&m_numMessages, sizeof( m_numMessages ) );
        return true;
    }
    else // error: packet still had enough room but couldn't write all msg data for some reason
        return false;
}