//=================================================================================
// NetworkSystem.hpp
// Author: Tyler George
// Date  : January 18, 2016
//=================================================================================

#pragma once

#ifndef __included_NetworkSystem__
#define __included_NetworkSystem__


///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <WinSock2.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <map>
#include "Engine/Networking/UDPSocket.hpp"
#include "NetworkAddress.hpp"

class NetworkSession;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
typedef std::map< short, NetworkSession* > SessionMap;

///---------------------------------------------------------------------------------
/// NetworkSystem class
///---------------------------------------------------------------------------------
class NetworkSystem
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    ~NetworkSystem();

    ///---------------------------------------------------------------------------------
    /// Startup/Shutdown
    ///---------------------------------------------------------------------------------
    bool Startup();
    void Shutdown();

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    static NetworkSystem* GetInstance();
    static std::string GetLocalHostName();
    static addrinfo* GetAddressesForHost( const std::string& hostName, const char* service, int addrFamily = AF_UNSPEC );
    static void ListAddressesForHost( const std::string& hostName, const char* service, int addrFamily = AF_UNSPEC );
    static NetworkAddress GetClientAddress( const std::string& hostName, const char* service, int addrFamily = AF_UNSPEC );
    static void* GetInAddress( sockaddr* socketAddress );
    static NetworkSession* DoesSessionExist( short port );

    ///---------------------------------------------------------------------------------
    /// Mutators
    ///---------------------------------------------------------------------------------
    NetworkSession* CreateSession( short port );
    UDPSocket* CreateUDPSocket( NetworkPacketQueue* queue, short port );

    // be sure to call .join on the socket before freeing the memory
    // otherwise you will have thread issues.
    void FreeSocket( UDPSocket* sock );


    static SOCKET StartHost( const std::string& hostName, const char* service, int addrFamily = AF_INET );
    static SOCKET StartClient( const std::string& hostName, const char* service, int addrFamily = AF_INET );

private:
    ///---------------------------------------------------------------------------------
    /// Private functions
    ///---------------------------------------------------------------------------------
    NetworkSystem();

    ///---------------------------------------------------------------------------------
    /// Private member variables
    ///---------------------------------------------------------------------------------
    static NetworkSystem* s_theNetworkSystem;
    SessionMap m_sessions;


};
#endif