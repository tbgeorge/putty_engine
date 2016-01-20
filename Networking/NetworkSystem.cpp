//=================================================================================
// NetworkSystem.cpp
// Author: Tyler George
// Date  : January 18, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "NetworkSystem.hpp"

////===========================================================================================
///===========================================================================================
// NetworkSystem Class
///===========================================================================================
////===========================================================================================

////===========================================================================================
///===========================================================================================
// Startup/Shutdown
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkSystem::Startup()
{
    WSADATA wsa_data;
    int error = WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );

    if (error == 0)
    {
        return true;
    }
    else
    {
        printf( "Failed to initialize WinSock. Error[%u]\n", error );
        return false;
    }
}

///---------------------------------------------------------------------------------
///
///--------------------------------------------------------------------------------- 
void NetworkSystem::Shutdown()
{
    WSACleanup();
}

////===========================================================================================
///===========================================================================================
// Accessors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
std::string NetworkSystem::GetLocalHostName()
{
    const int maxHostNameLen = 256; //from docs
    char cstrHostName[maxHostNameLen];
    int getHostNameCode = gethostname( cstrHostName, maxHostNameLen );

    if (getHostNameCode == SOCKET_ERROR)
        return std::string( "" );

    size_t hostNameLen = strlen( cstrHostName );
    if (hostNameLen == 0)
        return std::string( "" );

    std::string hostName( cstrHostName );
    return hostName;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
addrinfo* NetworkSystem::GetAddressesForHost( const std::string& hostName, const char* service, int addrFamily /* = AF_UNSPCE */ )
{
    addrinfo hints;
    addrinfo* addr;

    if (hostName.empty())
        return nullptr;

    // init hints
    memset( &hints, 0, sizeof( hints ) );

    // Which network layer it's using - usually want to UNSPEC, since it doesn't matter.  But since we're hard coding
    // the client sides connection, we will likely want to use AF_INET when we want to bind an address
    hints.ai_family = addrFamily;
    hints.ai_socktype = SOCK_STREAM; // STREAM based, determines transport layer (TCP)
    hints.ai_flags = AI_PASSIVE; // used for binding/listening

    int status = getaddrinfo( hostName.c_str(), service, &hints, &addr );
    if (status != 0)
    {
        printf( "%s: Failed to create socket address: %ws\n", hostName.c_str(), gai_strerror( status ) );
        return nullptr;
    }
    else
        return addr;
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkSystem::ListAddressesForHost( const std::string& hostName, const char* service, int addrFamily /* = AF_UNSPEC */ )
{
    addrinfo* hostAddresses = GetAddressesForHost( hostName, service, addrFamily );

    addrinfo* iter;
    for (iter = hostAddresses; iter != nullptr; iter = iter->ai_next)
    {
        char addrName[INET6_ADDRSTRLEN];
        inet_ntop( iter->ai_family, GetInAddress( iter->ai_addr ), addrName, INET6_ADDRSTRLEN );
        printf( "%s: Address family[%i] type[%i] %s : %s\n", hostName.c_str(), iter->ai_family, iter->ai_socktype, addrName, service );
    }

    freeaddrinfo( hostAddresses );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void* NetworkSystem::GetInAddress( sockaddr* socketAddress )
{
    if (socketAddress->sa_family == AF_INET) {
        return &(((sockaddr_in*)socketAddress)->sin_addr);
    }
    else {
        return &(((sockaddr_in6*)socketAddress)->sin6_addr);
    }
}

///---------------------------------------------------------------------------------
/// Has the possibility to return INVALID_SOCKET
///---------------------------------------------------------------------------------
SOCKET NetworkSystem::StartHost( const std::string& hostName, const char* service, int addrFamily /* = AF_INET */ )
{
    addrinfo* hostAddresses = GetAddressesForHost( hostName, service, addrFamily );

    SOCKET hostSocket = INVALID_SOCKET;

    addrinfo* iter;
    for (iter = hostAddresses; iter != nullptr; iter = iter->ai_next)
    {
        char addrName[INET6_ADDRSTRLEN];
        inet_ntop( iter->ai_family, GetInAddress( iter->ai_addr ), addrName, INET6_ADDRSTRLEN );
        printf( "%s: Trying to bind address: family[%i] type[%i] %s : %s\n", hostName.c_str(), iter->ai_family, iter->ai_socktype, addrName, service );

        hostSocket = socket( iter->ai_family, iter->ai_socktype, iter->ai_protocol );
        if (hostSocket == INVALID_SOCKET)
        {
            int error = WSAGetLastError();
            printf( "%s: Failed to create socket: Socket Error[%i]\n", hostName.c_str(), error );
            continue;
        }

        int bindResult = bind( hostSocket, iter->ai_addr, iter->ai_addrlen );
        if (bindResult == SOCKET_ERROR)
        {
            closesocket( hostSocket );
            int error = WSAGetLastError();
            printf( "%s: Failed to bind: Socket Error[%i]\n", hostName.c_str(), error );
            continue;
        }

        // successfuly bound socket to address
        printf( "%s: Socket bound to address: family[%i] type[%i] %s : %s\n", hostName.c_str(), iter->ai_family, iter->ai_socktype, addrName, service );
        break;
    }

    freeaddrinfo( hostAddresses );

    return hostSocket;
}

///---------------------------------------------------------------------------------
/// Has the possibility to return INVALID_SOCKET
///---------------------------------------------------------------------------------
SOCKET NetworkSystem::StartClient( const std::string& hostName, const char* service, int addrFamily /* = AF_INET */ )
{
    addrinfo* hostAddresses = GetAddressesForHost( hostName, service, addrFamily );

    SOCKET hostSocket = INVALID_SOCKET;

    addrinfo *iter;
    for (iter = hostAddresses; iter != nullptr; iter = iter->ai_next)
    {
        char addr_name[INET6_ADDRSTRLEN];
        inet_ntop( iter->ai_family, GetInAddress( iter->ai_addr ), addr_name, INET6_ADDRSTRLEN );
        printf( "Trying to bind addr: family[%i] type[%i] %s : %s\n", iter->ai_family, iter->ai_socktype, addr_name, service );

        hostSocket = socket( iter->ai_family, iter->ai_socktype, iter->ai_protocol );
        if (hostSocket == INVALID_SOCKET)
        {
            int error = WSAGetLastError();
            printf( "Failed to create socket: Socket Error[%i]\n", error );
            continue;
        }

        int result = connect( hostSocket, iter->ai_addr, iter->ai_addrlen );
        if (result == SOCKET_ERROR)
        {
            printf( "Failed to connect.\n" );
            closesocket( hostSocket );
            hostSocket = INVALID_SOCKET;
            continue;
        }
    }

    freeaddrinfo( hostAddresses );

    return hostSocket;
}