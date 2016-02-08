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
#include "Engine/Networking/NetworkPacket.hpp"
class NetworkMessageDefinition;
class NetworkConnection;

///---------------------------------------------------------------------------------
/// Constants
///---------------------------------------------------------------------------------
const size_t MESSAGE_MTU = 1024;

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
    NetworkMessage( uint8_t id );
    NetworkMessage( NetworkPacket& packet );
    NetworkMessage( const NetworkMessage& copy );
    ~NetworkMessage();

    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    static void RegisterNetworkMessageDefinition( NetworkMessageDefinition* definition );
    static void ProcessMessage( NetworkSender sender, NetworkMessage* msg );

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    NetworkMessageDefinition* FindMessageDefinitionById( uint8_t id );
    NetworkMessageDefinition* GetDefinition() const { return m_definition; }

private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    unsigned char* m_buffer;
    NetworkMessageDefinition* m_definition;


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
    NetworkMessageDefinition( uint8_t id, NetworkMessageCallbackFunc callback );

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    uint8_t GetID() const { return m_id; }
    NetworkMessageCallbackFunc GetCallback() const { return m_callbackFunc; }

private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    uint8_t m_id;
    NetworkMessageCallbackFunc m_callbackFunc;
};

///---------------------------------------------------------------------------------
/// Defines
///---------------------------------------------------------------------------------
#define NETWORK_MESSAGE_DEFINITION( id ) static void NetworkMessageCallback_##id##( NetworkSender sender, NetworkMessage& msg ); \
                                         static NetworkMessageDefinition MessageDefinition_##id##( (uint8_t)id, NetworkMessageCallback_##id## ); \
                                         static void NetworkMessageCallback_##id##( NetworkSender sender, NetworkMessage& msg )

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
typedef std::vector< NetworkMessage* > NetworkMessages;

#endif
