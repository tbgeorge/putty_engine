//=================================================================================
// UDPSocket.cpp
// Author: Tyler George
// Date  : January 25, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Networking/UDPSocket.hpp"
#include "Engine/Multi-Threading/Thread.hpp"
#include "Engine/Utilities/Error.hpp"
#include "Engine/Networking/NetworkPacketQueue.hpp"
#include "Engine/Networking/NetworkPacket.hpp"

#include <winsock2.h>
#include "Engine/Networking/NetworkSystem.hpp"
#include "Engine\Utilities\DeveloperConsole.hpp"
#include <ws2ipdef.h>
#include <WS2tcpip.h>

////===========================================================================================
///===========================================================================================
// SocketThread class definition
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class SocketThread : public Thread
{

public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ~SocketThread();

    ///---------------------------------------------------------------------------------
    /// Startup
    ///---------------------------------------------------------------------------------
    void Startup( UDPSocket* owner, NetworkPacketQueue* queue, short port );

protected:
    ///---------------------------------------------------------------------------------
    /// 
    ///---------------------------------------------------------------------------------
    virtual void Run();

private:
    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    SOCKET m_socket;
    short m_port;
    UDPSocket* m_owner;
    NetworkPacketQueue* m_packetQueue;

    ///---------------------------------------------------------------------------------
    /// Private functions
    ///---------------------------------------------------------------------------------
    void SetupSocket();
    void ProcessIncoming();
    void ProcessOutgoing();
    void ServiceSocket();
};

////===========================================================================================
///===========================================================================================
// SocketThread implementation
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
/// Constructors/Destructors
///---------------------------------------------------------------------------------
SocketThread::~SocketThread()
{

}

///---------------------------------------------------------------------------------
/// Startup
///---------------------------------------------------------------------------------
void SocketThread::Startup( UDPSocket* owner, NetworkPacketQueue* queue, short port )
{
    m_socket = INVALID_SOCKET;
    m_port = port;
    m_packetQueue = queue;
    m_owner = owner;

    NetworkSystem* sys = NetworkSystem::GetInstance();
    std::string portStr = std::to_string( m_port );
    const char* service = portStr.c_str();
    addrinfo* hostAddresses = sys->GetAddressesForHost( sys->GetLocalHostName(), service, AF_INET );

    addrinfo* iter = hostAddresses;
    m_owner->SetAddress( new NetworkAddress( iter->ai_addr, iter->ai_addrlen ) );

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SocketThread::Run()
{
    SetupSocket();
    ServiceSocket();
}

////===========================================================================================
///===========================================================================================
//  Private functions
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SocketThread::SetupSocket()
{
    NetworkSystem* sys = NetworkSystem::GetInstance();
    std::string portStr = std::to_string( m_port );
    const char* service = portStr.c_str();
    addrinfo* hostAddresses = sys->GetAddressesForHost( sys->GetLocalHostName(), service, AF_INET );

    addrinfo* iter = hostAddresses;

    char addrName[INET6_ADDRSTRLEN];
    void* address = sys->GetInAddress( iter->ai_addr );
    inet_ntop( iter->ai_family, address, addrName, INET6_ADDRSTRLEN );

    m_socket = INVALID_SOCKET;

    // 1. create the socket
    SOCKET sock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
    FATAL_ASSERT( sock != INVALID_SOCKET ); // TODO: make better error handling


    // 2. bind the socket to whatever address is available, on the port supplied
    // setup the address to use
    sockaddr_in addr;
    memset( &addr, 0, sizeof( addr ) );
    addr.sin_addr.S_un.S_addr = *((uint32_t*)address);
    addr.sin_port = htons( m_port );
    addr.sin_family = PF_INET;

    int status = bind( sock, (sockaddr*)&addr, sizeof( addr ) );
    if (status == SOCKET_ERROR)
    {
        DeveloperConsole::WriteLine( ERROR_TEXT_COLOR, "Could not bind socket to address: %s : %s ", addrName, service );
        //failed to bind address; TODO: handle errors here
        closesocket( sock );
        return;
    }
    
    DeveloperConsole::WriteLine( INFO_TEXT_COLOR, "Bound to address: %s : %s", addrName, service );

    m_socket = sock;

    // Set to non_blocking
    unsigned long non_blocking = 1;
    ioctlsocket( m_socket, FIONBIO, &non_blocking );

    freeaddrinfo( hostAddresses );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SocketThread::ProcessIncoming()
{
    // Try to recv data, read until nothing there
    // or it erred out
    unsigned char buffer[PACKET_MTU];

    sockaddr_storage their_addr;

    while (m_owner->IsRunning())
    {
        size_t addrlen = sizeof( their_addr );
        int recvd = recvfrom( m_socket, (char*)buffer, PACKET_MTU, 0, (sockaddr*)&their_addr, (int*)&addrlen );

        if (recvd > 0)
            m_packetQueue->EnqueueRead( buffer, recvd, (sockaddr*)&their_addr, addrlen );

        else if (recvd < 0)
        {
            int error = WSAGetLastError();

            switch (error)
            {
            case WSAEWOULDBLOCK: // nothing to read
            case WSAEMSGSIZE: // was too large, truncated, throw it out
                break;
            default:
                m_owner->Close();
                break;
            };

            return;
        }
        
        else
            m_owner->Close();
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SocketThread::ProcessOutgoing()
{
    NetworkPacket* packet = m_packetQueue->DequeueWrite();
    while (packet != nullptr)
    {
        NetworkAddress* address = packet->GetAddress();
        sendto( m_socket, (char*)packet->GetBuffer(), packet->GetLength(), 0, (sockaddr*)&address->m_addr, address->addrlen );
        delete packet;

        packet = m_packetQueue->DequeueWrite();
    }
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void SocketThread::ServiceSocket()
{
    while( m_owner->IsRunning() )
    {
        ProcessIncoming();
        ProcessOutgoing();
        yield();
    }
}

////===========================================================================================
///===========================================================================================
// UDPSocket implementation
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
UDPSocket::UDPSocket( NetworkPacketQueue* queue, short port )
    : m_running( true )
{
    m_serviceThread = new SocketThread();

    m_serviceThread->Startup( this, queue, port );
    m_serviceThread->Start( "socket thread" );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
UDPSocket::~UDPSocket()
{
    m_serviceThread->Join();
    delete m_serviceThread;

    delete m_boundAddress;
}

////===========================================================================================
///===========================================================================================
// Mutators
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
/// Blocks this socket until it's thread is finished
///---------------------------------------------------------------------------------
void UDPSocket::Join()
{
    Close();
    m_serviceThread->Join();
}