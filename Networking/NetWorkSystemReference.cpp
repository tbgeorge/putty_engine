// First, Broadcast Example
typedef unsigned char byte_t;

CNetSystem; // This controls startup/shutdown engine tasks (WSAInit, WSACleanup, managing threads, managing jobs)

CNetSession; // This is a collection of connections, for what they are logically

CNetConnection; // All connection based info - identifier, flow control, owning session, etc...

CNetPacket; // Byte Buffer of message we get and send to the socket, made up of messages

CNetMessage; // Single concept that is sent over, references a CNetMessageDefinition

CNetMessageDefinition; // Callback, options, etc for a net message

CByteBuffer; // Utility for writing to an array of bytes

CObjectPool; // Optimization - used for allocating and deallocating messages and packets

CPacketQueue; // Thread safe queue for both read & write of packets to a socket

// Internal
CUDPSocket; // wraps a single UDPSocket

CSocketWriteThread; // 

CSocketReadThread; //




void CGame::StartHosting( short port )
{
	NetworkSystem* sys = NetworkSystem::GetInstance();
	NetworkSession* session = sys->CreateSession();

	if( session->host(port)) 
	{
		m_gameSession = session;
		ConsolePrintf( "Game Hosted: ..." );
		m_gameSession->listen( true );
		
	}
}







Connection::NetworkTick()
{
	msg* to_send;
	if( reliable )
		msg.reliable_id = next_reliable_id++;

	packet.add( msg );
}

T CycleMax<T>( T a, T b )
{
	T d = a - b;
	if( d < ( MAX_VALUE( T ) / (T)2 ) )
		return a;
	else
		return b;
}