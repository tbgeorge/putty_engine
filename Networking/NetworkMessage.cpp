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
NetworkMessageDefinition::NetworkMessageDefinition( uint8_t id, NetworkMessageCallbackFunc callback, uint32_t options /* = 0 */ )
    : m_id( id )
    , m_callbackFunc( callback )
    , m_options( options )
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
NetworkMessage::NetworkMessage()
    : m_definition( nullptr )
    , m_buffer( nullptr )
    , m_lastTimeSent( 0.0 )
    , m_msgLen( 0 )
    , m_reliableID( MESSAGE_INVALID_RELIABLE_ID )
    , m_orderID( MESSAGE_INVALID_ORDER_ID )
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkMessage::NetworkMessage( uint8_t id )
    : m_definition( nullptr )
    , m_buffer( nullptr )
    , m_lastTimeSent( 0.0 )
    , m_msgLen( 0 )
    , m_reliableID( MESSAGE_INVALID_RELIABLE_ID )
    , m_orderID( MESSAGE_INVALID_ORDER_ID )
{
    m_definition = FindMessageDefinitionById( id );
    FATAL_ASSERT( m_definition != nullptr );

    // allocate my buffer
    m_buffer = new unsigned char[MESSAGE_MTU];
    ByteBuffer::Startup( m_buffer, MESSAGE_MTU );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkMessage::NetworkMessage( NetworkPacket* packet, bool& out_successful )
    : m_definition( nullptr )
    , m_buffer( nullptr )
    , m_lastTimeSent( 0.0 )
    , m_msgLen( 0 )
    , m_reliableID( MESSAGE_INVALID_RELIABLE_ID )
    , m_orderID( MESSAGE_INVALID_ORDER_ID )
{
    out_successful = InitFromPacket( packet );
    
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkMessage::NetworkMessage( const NetworkMessage& copy )
    : m_definition( nullptr )
    , m_buffer( nullptr )
    , m_lastTimeSent( 0.0 )
    , m_reliableID( MESSAGE_INVALID_RELIABLE_ID )
    , m_msgLen( 0 )
    , m_orderID( MESSAGE_INVALID_ORDER_ID )
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
// Initialization
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkMessage::InitFromPacket( NetworkPacket* packet )
{
    m_msgLen = 0;
    m_definition = nullptr;
    
    size_t currentReadLength = packet->m_readIndex;

    // get length
    uint16_t totalSize;
    size_t amtRead = packet->ReadBytes( &totalSize, sizeof( uint16_t ) );
    
    // no more data in packet
    if (amtRead == 0)
        return false;

    // if read message len is larger than is left to read from the packet
    size_t remainingData = packet->m_maxSize - packet->m_readIndex;
    if (m_msgLen > (uint16_t)remainingData)
        return false;

    // get id
    uint8_t id;
    packet->ReadBytes( &id, sizeof( uint8_t ) );
    
    // find definition
    m_definition = FindMessageDefinitionById( id );
    // definition does not exist
    if (!m_definition)
        return false;

    // if reliable, read reliable id
    if (m_definition->IsReliable())
        packet->ReadBytes( &m_reliableID, sizeof( uint16_t ) );

    // if in-order, read order id
    if (m_definition->IsInOrder())
        packet->ReadBytes( &m_orderID, sizeof( uint16_t ) );

    size_t headerLen = packet->m_readIndex - currentReadLength;
    m_msgLen = totalSize - (uint16_t)headerLen;

    // use the packet's buffer
    ByteBuffer::Startup( (void*)(packet->GetBuffer() + packet->m_readIndex), m_msgLen );

    // move the packet's read index ahead
    packet->m_readIndex += (m_msgLen);

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

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------    
bool NetworkMessage::WriteString( const char* str )
{
    if (ByteBuffer::WriteString( str ))
        return true;
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkMessage::WriteBytes( void* data, size_t size )
{
    if (ByteBuffer::WriteBytes( data, size ))
    {
        m_msgLen += (uint16_t)size;
        return true;
    }
    return false;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkMessage::operator=( const NetworkMessage& msg )
{
    m_definition = msg.m_definition;
    m_maxSize = msg.m_maxSize;
    m_msgLen = msg.m_msgLen;
    m_orderID = msg.m_orderID;
    m_reliableID = msg.m_reliableID;
    
    WriteBytes( msg.m_buf, m_msgLen );
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

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
size_t NetworkMessage::GetHeaderSize() const
{
    return sizeof( uint16_t ) // length
        + sizeof( uint8_t ) // message type id
        + (m_definition->IsReliable() ? 2 : 0) // reliable id IF msg is reliable
        + (m_definition->IsInOrder() ? 2 : 0); // order id IF msg is in-order
}


////===========================================================================================
///===========================================================================================
// Message Definitions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
/// Ping
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( MSG_TYPE_PING, MO_CONNECTIONLESS )
{
    UNUSED( msg );
    DeveloperConsole::WriteLine( INFO_TEXT_COLOR, "Ping received from %s", sender.m_addr.GetAddrStr().c_str() );
    // send pong back

    NetworkMessage* pongMsg = new NetworkMessage( 1 );
    sender.m_session->SendDirectMessage( pongMsg, &sender.m_addr );
}

///---------------------------------------------------------------------------------
/// Pong
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( MSG_TYPE_PONG, MO_CONNECTIONLESS )
{
    UNUSED( msg );
    DeveloperConsole::WriteLine( INFO_TEXT_COLOR, "Pong received from %s", sender.m_addr.GetAddrStr().c_str() );
}

///---------------------------------------------------------------------------------
/// Heartbeat
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( MSG_TYPE_HEARTBEAT, MO_CONNECTIONLESS )
{
    UNUSED( msg );

//     NetworkConnection* conn = sender.m_connection;
//     //
//     if (conn == nullptr)
//         return;

    DeveloperConsole::WriteLine( INFO_TEXT_COLOR, "Heartbeat received from %s", sender.m_addr.GetAddrStr().c_str() );
}

///---------------------------------------------------------------------------------
/// Ack
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION( MSG_TYPE_ACK )
{
    NetworkConnection* conn = sender.m_connection;
    // received an ack from an unknown connection
    if (conn == nullptr)
        return;

    uint8_t ackCount;
    msg.ReadBytes( &ackCount, sizeof( ackCount ) );

    while (ackCount > 0)
    {
        uint16_t ack;
        msg.ReadBytes( &ack, sizeof( ack ) );

        ReliableTracker* tracker = conn->FindAndRemoveTracker( ack );
        if (tracker != nullptr)
        {
            for (std::vector< uint16_t >::iterator idIter = tracker->m_reliableIDs.begin(); idIter != tracker->m_reliableIDs.end(); ++idIter)
            {
                uint16_t reliableID = *idIter;
                conn->GetSentReliableIDs().Unset( reliableID );
            }

            delete tracker;
        }
        --ackCount;
    }
}

///---------------------------------------------------------------------------------
/// Join Request
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( MSG_TYPE_JOIN_REQUEST, MO_RELIABLE )
{
    DeveloperConsole::WriteLine( "Join request from " + sender.m_addr.GetAddrStr(), INFO_TEXT_COLOR );

    char* id = nullptr;
    msg.ReadString( id );

    AddConnectionResult result = ACR_DENIED;
    NetworkConnection* conn = sender.m_session->AddConnection( sender.m_addr, std::string( id ), result );

    if (conn && result == ACR_ACCEPTED)
    {

        NetworkMessage acceptMsg( MSG_TYPE_JOIN_ACCEPT );

        uint8_t maxConnections = (uint8_t) sender.m_session->GetMaxConnections();
        acceptMsg.WriteBytes( (void*)&maxConnections, sizeof( maxConnections ) );

        uint8_t connIdx = conn->GetConnectionIndex();
        acceptMsg.WriteBytes( (void*)&connIdx, sizeof( connIdx ) );

        uint8_t hostIdx = 0;
        acceptMsg.WriteBytes( (void*)&hostIdx, sizeof( hostIdx ) );

        std::string hostID = sender.m_session->GetHost()->GetUniqueID();
        acceptMsg.WriteString( hostID.c_str() );

        uint8_t invalidIdx = CONNECTION_INVALID_INDEX;
        acceptMsg.WriteBytes( (void*)&invalidIdx, sizeof( invalidIdx ) );

        conn->AddMessage( acceptMsg );
    }
    else
    {
        uint8_t denyReason;
        NetworkMessage* denyMsg = new NetworkMessage( MSG_TYPE_JOIN_DENY );
        switch (result)
        {
        case ACR_DENIED:
            denyReason = 0;
            break;
        case ACR_FULL:
            denyReason = 1;
            break;
        case ACR_ID_TAKEN:
            denyReason = 2;
            break;
        default:
            break;
        }

        denyMsg->WriteBytes( (void*)&denyReason, sizeof( denyReason ) );
        sender.m_session->SendDirectMessage( denyMsg, &sender.m_addr );
    }
}

///---------------------------------------------------------------------------------
/// Join Accept
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( MSG_TYPE_JOIN_ACCEPT, MO_RELIABLE )
{
    // recieved from the connection I'm trying to join
    if (sender.m_connection && sender.m_session->GetState() == NETWORK_DISCONNECTED )
    {
        DeveloperConsole::WriteLine( "Join accepted", INFO_TEXT_COLOR );

        uint8_t maxConnections;
        msg.ReadBytes( &maxConnections, sizeof( maxConnections ) );

        uint8_t myConnectionIndex;
        msg.ReadBytes( &myConnectionIndex, sizeof( myConnectionIndex ) );

        sender.m_session->SetMyConnectionIndex( myConnectionIndex );
    }
    
}

///---------------------------------------------------------------------------------
/// Join Deny
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( MSG_TYPE_JOIN_DENY, MO_CONNECTIONLESS )
{
    if (sender.m_connection && sender.m_session->GetState() == NETWORK_DISCONNECTED)
    {
        uint8_t denyReason;
        msg.ReadBytes( &denyReason, sizeof( denyReason ) );

        switch (denyReason)
        {
        case 0:
            DeveloperConsole::WriteLine( "Join denied: Host is not joinable", INFO_TEXT_COLOR );
            break;
        case 1:
            DeveloperConsole::WriteLine( "Join denied: Host is full", INFO_TEXT_COLOR );
            break;
        case 2:
            DeveloperConsole::WriteLine( "Join denied: Unique ID is taken", INFO_TEXT_COLOR );
            break;
        default:
            DeveloperConsole::WriteLine( "Join denied: Unknown reason", INFO_TEXT_COLOR );
            break;
        }

        sender.m_session->DisconnectFromHost();
    }
}

///---------------------------------------------------------------------------------
/// Leave
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION( MSG_TYPE_LEAVE )
{
    UNUSED( msg );

    if (sender.m_session->RemoveConnection( sender.m_addr ))
        DeveloperConsole::WriteLine( "Connection leaving address: " + sender.m_connection->GetUniqueID(), INFO_TEXT_COLOR );
    else
        DeveloperConsole::WriteLine( "Received Leave message from unknown connection", INFO_TEXT_COLOR );
}

///---------------------------------------------------------------------------------
/// Start In-Order Test
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( MSG_TYPE_START_TEST, MO_RELIABLE )
{
    uint32_t numMsgs;
    msg.ReadBytes( (void*)&numMsgs, sizeof( numMsgs ) );

    DeveloperConsole::WriteLine( "Starting test of In-Order traffic with " + std::to_string( numMsgs ) + " messages... ", INFO_TEXT_COLOR );

    sender.m_connection->SetNumTestInOrderMsgsToSend( numMsgs );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( MSG_TYPE_IN_ORDER_TEST, MO_RELIABLE | MO_IN_ORDER )
{
    uint32_t msgVal;
    msg.ReadBytes( (void*)&msgVal, sizeof( msgVal ) );

    uint16_t orderID = msg.GetOrderID();

    DeveloperConsole::WriteLine( "Recieved In-Order test msg with value of " + std::to_string( msgVal ) + " and order ID of " + std::to_string( orderID ), INFO_TEXT_COLOR );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NETWORK_MESSAGE_DEFINITION_WITH_OPTIONS( MSG_TYPE_FORCE_TEST, MO_RELIABLE )
{
    uint32_t numMsgs;
    size_t bytesRead = msg.ReadBytes( (void*)&numMsgs, sizeof( numMsgs ) );

    if (bytesRead < 2)
        numMsgs = 8;

    NetworkMessage startMsg( MSG_TYPE_START_TEST );
    msg.WriteBytes( &numMsgs, sizeof( numMsgs ) );

    sender.m_connection->AddMessage( startMsg );

    DeveloperConsole::WriteLine( "Forcing test of in-order traffic", INFO_TEXT_COLOR );
}