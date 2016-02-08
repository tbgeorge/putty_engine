//=================================================================================
// NetworkMessage.cpp
// Author: Tyler George
// Date  : February 3, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Networking/NetworkMessage.hpp"
#include "Engine/Utilities/Error.hpp"
#include "Engine/Utilities/DeveloperConsole.hpp"
#include "NetworkConnection.hpp"
#include "NetworkAddress.hpp"
#include "NetworkSession.hpp"

////===========================================================================================
///===========================================================================================
// Static variable initialization
///===========================================================================================
////===========================================================================================

NetworkMessageDefinitionMap NetworkMessage::s_messageDefinitions;



////===========================================================================================
///===========================================================================================
// NetworkMessageDefinition class
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
NetworkMessageDefinition::NetworkMessageDefinition( uint8_t id, NetworkMessageCallbackFunc callback )
    : m_id( id )
    , m_callbackFunc( callback )
{
    NetworkMessage::RegisterNetworkMessageDefinition( this );
}


////===========================================================================================
///===========================================================================================
// NetworkMessage class
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
NetworkMessage::NetworkMessage( uint8_t id )
    : m_definition( nullptr )
    , m_buffer( nullptr )
{
    m_definition = FindMessageDefinitionById( id );
    FATAL_ASSERT( m_definition != nullptr );

    // allocate my buffer
    m_buffer = new unsigned char[MESSAGE_MTU];
    ByteBuffer::Startup( m_buffer, MESSAGE_MTU );

    // write message id
    uint16_t size = sizeof( uint16_t ) + sizeof( uint8_t );
    WriteBytes( (void*)&size, sizeof( size ) );
    WriteBytes( (void*)&id, sizeof( id ) );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkMessage::NetworkMessage( NetworkPacket& packet )
    : m_definition( nullptr )
    , m_buffer( nullptr )
{
    // get length and id
    uint16_t msgLen;
    packet.ReadBytes( &msgLen, sizeof( uint16_t ) );
    uint8_t id;
    packet.ReadBytes( &id, sizeof( uint8_t ) );

    // find definition
    m_definition = FindMessageDefinitionById( id );
//     FATAL_ASSERT( m_definition != nullptr );

    // use the packet's buffer
    ByteBuffer::Startup( (void*)(packet.GetBuffer() + packet.m_readIndex), msgLen );

    // move the packet's read index ahead
    packet.m_readIndex += (msgLen - (sizeof( uint16_t ) + sizeof( uint8_t )));



}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkMessage::NetworkMessage( const NetworkMessage& copy )
    : m_definition( nullptr )
    , m_buffer( nullptr )
{
    m_definition = copy.GetDefinition();

    m_buffer = new unsigned char[MESSAGE_MTU];
    ByteBuffer::Startup( m_buffer, MESSAGE_MTU );
    WriteBytes( copy.m_buf, copy.GetLength() );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkMessage::~NetworkMessage()
{
    if (m_buffer != nullptr)
        delete m_buffer;
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkMessage::RegisterNetworkMessageDefinition( NetworkMessageDefinition* definition )
{
    NetworkMessageDefinitionMap::iterator definitionIter = s_messageDefinitions.find( definition->GetID() );

    if (definitionIter != s_messageDefinitions.end())
    {
        // definition for that id already exists, don't add it
        return;
    }

    s_messageDefinitions.insert( std::pair< uint8_t, NetworkMessageDefinition* >( definition->GetID(), definition ) );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkMessage::ProcessMessage( NetworkSender sender, NetworkMessage* msg )
{
    NetworkMessageDefinition* def = msg->GetDefinition();
    def->GetCallback()(sender, *msg);

}

////===========================================================================================
///===========================================================================================
// Accessors
///===========================================================================================
////===========================================================================================


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkMessageDefinition* NetworkMessage::FindMessageDefinitionById( uint8_t id )
{
    NetworkMessageDefinitionMap::iterator definitionIter = s_messageDefinitions.find( id );

    if (definitionIter != s_messageDefinitions.end())
    {
        return definitionIter->second;
    }

    return nullptr;
}




////===========================================================================================
///===========================================================================================
// Message Definitions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
/// Ping
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION( 0 )
{
    DeveloperConsole::WriteLine( INFO_TEXT_COLOR, "Ping received from %s", sender.m_addr.GetAddrStr().c_str() );
    // send pong back

    NetworkMessage* pongMsg = new NetworkMessage( 1 );
    sender.m_session->SendDirectMessage( pongMsg, &sender.m_addr );
}

///---------------------------------------------------------------------------------
/// Pong
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION( 1 )
{
    DeveloperConsole::WriteLine( INFO_TEXT_COLOR, "Pong received from %s", sender.m_addr.GetAddrStr().c_str() );
}

///---------------------------------------------------------------------------------
/// Heartbeat
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION( 3 )
{
    DeveloperConsole::WriteLine( INFO_TEXT_COLOR, "Heartbeat recieved from %s", sender.m_addr.GetAddrStr().c_str() );
}