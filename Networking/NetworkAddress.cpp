//=================================================================================
// NetworkAddress.cpp
// Author: Tyler George
// Date  : January 25, 2016
//=================================================================================

////===========================================================================================
///===========================================================================================
// Includes
///===========================================================================================
////===========================================================================================
#include "Engine/Networking/NetworkAddress.hpp"
#include <memory.h>
#include <WS2tcpip.h>
#include "NetworkSystem.hpp"

////===========================================================================================
///===========================================================================================
// Constructors/Destructors
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkAddress::NetworkAddress()
{

}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkAddress::NetworkAddress( sockaddr* addr, size_t addrLen )
{
    Startup( addr, addrLen );
}

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
NetworkAddress::~NetworkAddress()
{
}

////===========================================================================================
///===========================================================================================
// Initialization
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
void NetworkAddress::Startup( sockaddr* addr, size_t addrLen )
{
//     addr = *((sockaddr_storage*)saddr);
    memcpy( m_addr, addr, addrLen );
    this->addrlen = addrLen;


    char addrName[INET6_ADDRSTRLEN];
    inet_ntop( AF_INET, NetworkSystem::GetInAddress( addr ), addrName, addrLen );
    m_addrStr = std::string( addrName );
}

////===========================================================================================
///===========================================================================================
// Accessors/Queries
///===========================================================================================
////===========================================================================================

///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
bool NetworkAddress::Equals( const NetworkAddress& addr )
{
    if (addrlen != addr.addrlen)
        return false;
    else
    {
        int result = memcmp( m_addr, addr.m_addr, addrlen );
        if (result == 0)
            return true;
        else
            return false;
    }
}