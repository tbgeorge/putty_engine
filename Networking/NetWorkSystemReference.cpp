
// Step 1: Add a check for disconnect method
// Step 2: Implement CheckForDisconnect
// Step 3: Add appropriate members methods
// Step 4: Update last recieved

// HEARTBEAT
// Step 1: In NetworkTick - check if we should send a heartbeat
// Step 2: Define a heartbeat message

#define cMessage_Ping 0
#define cMessgae_Pong 1
#define cMessage_Heartbeat 2

// Should already exist - called on a network tick per connection
void NetworkConnection::NetworkTick()
{
	this->CheckForDisconnect();
	if( !this->IsConnected() )
	{
		return;
	}

	if( ShouldSendHeartbeat() ) 
	{
		SendHeartbeat();
	}

	this->SendPacket();
}

void NetworkConnection::ProcessPacket( NetworkPacket& packet )
{
	m_timeLastReceivedPacket = TimeGetSeconds();
}

// NEW
void NetworkConnection::CheckForDisconnect()
{
	if( IsMe() )
	{
		return;
	}

	double currentTime = TimeGetSeconds();
	double age = currentTime - this->TimeLastReceivedPacket;
	if( age > CONNECTION_TIMEOUT )
	{
		SetState( eConnectionState_Disconnected );
	}
}

// NEW
bool NetworkConnection::ShouldSendHeartbeat()
{
	if( IsMe() )
	{
		return false;
	}

	double currentTime = TimeGetSeconds();
	double age = currentTime - this->m_timeLastSentHeartbeat;
	if( age > CONNECTION_HEARTBEAT_TIME )
	{
		return true;
	}
	return false;
}

void NetworkConnection::SendHeartbeat()
{
	// However you send messages to your connection
	NetworkMessage msg( /* Heartbeat ID */ );
	this->SendMessage( msg );

	// Update clock
	m_timeLastSentHeartbeat = TimeGetSeconds();
}

//======= HEADER FILE =========================================//
double const CONNECTION_TIMEOUT = 30.0;
double const CONNECTION_HEARTBEAT_TIME = 2.5;

enum eConnectionState
{
	eConnectionState_Connected, // starts in this state
	eConnectionState_Disconnected,
};

class NetworkConnection
{
	// Things we need to add for this to work
	double m_timeLastReceivedPacket;
	double m_timeLastSentHeartbeat;
	eConnectionState m_state;

	void CheckForDisconnect();
	void SetState( eConnectionState state ) { m_state = state; }

	bool IsMe() const { return m_session->me == this; } // How Forseth implemented this

}