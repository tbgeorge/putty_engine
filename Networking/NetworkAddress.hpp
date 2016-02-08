//=================================================================================
// NetworkAddress.hpp
// Author: Tyler George
// Date  : January 25, 2016
//=================================================================================
#pragma once

#ifndef __included_NetworkAddress__
#define __included_NetworkAddress__

///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include <string>
struct sockaddr;
class NetworkSession;
class NetworkConnection;

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class NetworkAddress
{
public:
    ///---------------------------------------------------------------------------------
    /// Constructors/Destructors
    ///---------------------------------------------------------------------------------
    NetworkAddress();
    NetworkAddress( sockaddr* addr, size_t addrLen );
    ~NetworkAddress();

    ///---------------------------------------------------------------------------------
    /// Initialization
    ///---------------------------------------------------------------------------------
    void Startup( sockaddr* addr, size_t addrLen );

    ///---------------------------------------------------------------------------------
    /// Accessors/Queries
    ///---------------------------------------------------------------------------------
    bool Equals( const NetworkAddress& addr );
    std::string GetAddrStr() { return m_addrStr; }


    ///---------------------------------------------------------------------------------
    /// Public Member variables
    ///---------------------------------------------------------------------------------
    unsigned char m_addr[16];
    std::string m_addrStr;
    size_t addrlen;

};

struct NetworkSender
{
    NetworkAddress m_addr;
    NetworkSession* m_session;
    NetworkConnection* m_connection;

    NetworkSender( const NetworkAddress& addr, NetworkSession* session, NetworkConnection* connection )
        : m_addr( addr ), m_session( session ), m_connection( connection ) {}
};

#endif
