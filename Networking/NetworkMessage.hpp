//=================================================================================
// NetworkMessage.hpp
// Author: Tyler George
// Date  : February 3, 2016
//=================================================================================
#pragma once

#ifndef __included_NetworkMessage__
#define __included_NetworkMessage__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <stdint.h>
#include <map>
#include <queue>
#include "Engine/Networking/NetworkPacket.hpp"
class NetworkMessageDefinition;
class NetworkConnection;

///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
const size_t MESSAGE_MTU = 1024;
const double MESSAGE_RESEND_TIME = 0.1;
const uint16_t MESSAGE_INVALID_RELIABLE_ID = 0xFFFF;
const uint16_t MESSAGE_INVALID_ORDER_ID = 0xFFFF;

///---------------------------------------------------------------------------------
/// Enums
///---------------------------------------------------------------------------------
enum CoreMessageTypes : uint8_t
{
    MSG_TYPE_PING,
    MSG_TYPE_PONG,
    MSG_TYPE_HEARTBEAT,
    MSG_TYPE_ACK,
    MSG_TYPE_JOIN_REQUEST,
    MSG_TYPE_JOIN_ACCEPT,
    MSG_TYPE_JOIN_DENY,
    MSG_TYPE_LEAVE,
    MSG_TYPE_START_TEST,
    MSG_TYPE_IN_ORDER_TEST,
    MSG_TYPE_FORCE_TEST,
    NUM_CORE_MSG_TYPES
};

enum MessageOptions
{
    MO_RELIABLE = (1 << 0),
    MO_CONNECTIONLESS = (1 << 1),
    MO_IN_ORDER = ( 1 << 2 ),
};

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------
typedef std::map< uint8_t, NetworkMessageDefinition* > NetworkMessageDefinitionMap;

///---------------------------------------------------------------------------------
/// NetworkMessage class
///---------------------------------------------------------------------------------
class NetworkMessage : public ByteBuffer
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    NetworkMessage();
    NetworkMessage( uint8_t id );
    NetworkMessage( NetworkPacket* packet, bool& out_successful );
    NetworkMessage( const NetworkMessage& copy );
    ~NetworkMessage();

    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------
    bool InitFromPacket( NetworkPacket* packet );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    static void RegisterNetworkMessageDefinition( NetworkMessageDefinition* definition );
    static void ProcessMessage( NetworkSender sender, NetworkMessage* msg );
    void SetReliableID( uint16_t reliableID ) { m_reliableID = reliableID; }
    void SetOrderID( uint16_t orderID ) { m_orderID = orderID; }
    void SetLastTimeSent( double time ) { m_lastTimeSent = time; }
    virtual bool WriteString( const char* str );
    virtual bool WriteBytes( void* data, size_t size );

    void operator=( const NetworkMessage& msg );

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    NetworkMessageDefinition* FindMessageDefinitionById( uint8_t id );
    NetworkMessageDefinition* GetDefinition() const { return m_definition; }
    uint16_t GetReliableID() const { return m_reliableID; }
    uint16_t GetOrderID() const { return m_orderID; }
    double GetLastTimeSent() const { return m_lastTimeSent; }
    size_t GetHeaderSize() const;
    size_t GetMessageSize() const { return m_msgLen; }

private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    unsigned char* m_buffer;
    NetworkMessageDefinition* m_definition;
    uint16_t m_msgLen;
    uint16_t m_reliableID;
    uint16_t m_orderID;
    double m_lastTimeSent;

    static NetworkMessageDefinitionMap s_messageDefinitions;
};

///---------------------------------------------------------------------------------
/// Typedefs
///---------------------------------------------------------------------------------
typedef void( *NetworkMessageCallbackFunc )(NetworkSender sender, NetworkMessage& msg);


///---------------------------------------------------------------------------------
/// NetworkMessageDefinition class
///---------------------------------------------------------------------------------
class NetworkMessageDefinition
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    NetworkMessageDefinition( uint8_t id, NetworkMessageCallbackFunc callback, uint32_t options = 0 );

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    uint8_t GetID() const { return m_id; }
    NetworkMessageCallbackFunc GetCallback() const { return m_callbackFunc; }
    uint32_t GetOptions() const { return m_options; }

    bool IsConnectionless() { return (m_options & MO_CONNECTIONLESS) != 0; }
    bool IsReliable() { return (m_options & MO_RELIABLE) != 0; }
    bool IsInOrder() { return (m_options & MO_IN_ORDER) != 0; }

private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    uint8_t m_id;
    NetworkMessageCallbackFunc m_callbackFunc;
    uint32_t m_options;
};

///---------------------------------------------------------------------------------
/// Defines
///---------------------------------------------------------------------------------
#define NETWORK_MESSAGE_DEFINITION( id ) static void NetworkMessageCallback_##id##( NetworkSender sender, NetworkMessage& msg ); \
                                         static NetworkMessageDefinition MessageDefinition_##id##( (uint8_t)id, NetworkMessageCallback_##id##, (uint32_t)0 ); \
                                         static void NetworkMessageCallback_##id##( NetworkSender sender, NetworkMessage& msg )

#define NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( id, options ) static void NetworkMessageCallback_##id##( NetworkSender sender, NetworkMessage& msg ); \
                                         static NetworkMessageDefinition MessageDefinition_##id##( (uint8_t)id, NetworkMessageCallback_##id##, (uint32_t)options ); \
                                         static void NetworkMessageCallback_##id##( NetworkSender sender, NetworkMessage& msg )

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
typedef std::vector< NetworkMessage* > NetworkMessages;
typedef std::queue<NetworkMessage*> NetworkMessageQueue;

#endif
