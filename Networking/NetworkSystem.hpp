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
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <conio.h>
#include <string>

///---------------------------------------------------------------------------------
/// NetworkSystem class
///---------------------------------------------------------------------------------
class NetworkSystem
{
public:
    ///---------------------------------------------------------------------------------
    /// Startup/Shutdown
    ///---------------------------------------------------------------------------------
    bool Startup();
    void Shutdown();

    ///---------------------------------------------------------------------------------
    /// Accessors
    ///---------------------------------------------------------------------------------
    static std::string GetLocalHostName();
    static addrinfo* GetAddressesForHost( const std::string& hostName, const char* service, int addrFamily = AF_UNSPEC );
    static void ListAddressesForHost( const std::string& hostName, const char* service, int addrFamily = AF_UNSPEC );
    static void* GetInAddress( sockaddr* socketAddress );

    static SOCKET StartHost( const std::string& hostName, const char* service, int addrFamily = AF_INET );
    static SOCKET StartClient( const std::string& hostName, const char* service, int addrFamily = AF_INET );
};


#endif