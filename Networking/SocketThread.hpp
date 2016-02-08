//=================================================================================
// SocketThread.hpp
// Author: Tyler George
// Date  : January 25, 2016
//=================================================================================
#pragma once

#ifndef __included_SocketThread__
#define __included_SocketThread__


///---------------------------------------------------------------------------------
/// Includes
///---------------------------------------------------------------------------------
#include "Engine\Multi-Threading\Thread.hpp"
#include "Engine\Utilities\Error.hpp"
#include <winsock.h>
#include <ws2def.h>
#include "NetworkSystem.hpp"


///---------------------------------------------------------------------------------
///
///---------------------------------------------------------------------------------
class SocketThread : public Thread
{
private:
    SOCKET m_socket;
    short port;
    UDPSocket* owner;
    NetworkPacketQueue* packetQueue;

    void SetupSocket()
    {
        m_socket = INVALID_SOCKET;

        // 1. create the socket
        
        SOCKET sock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
        FATAL_ASSERT( sock != INVALID_SOCKET ); // TODO: make better error handling

        // 2. bind the socket to whatever address is available, on the port supplied
        //
        // setup the address to use
        sockaddr_in addr;
        memset( &addr, 0, sizeof( addr ) );
        addr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
        addr.sin_port = htons( port );
        addr.sin_family = PF_INET;

        int status = bind( sock, (sockaddr*)&addr, sizeof( addr ) );
        if (status == SOCKET_ERROR)
        {
            //failed to bind address; TODO: handle errors here
            closesocket( sock );
            return;
        }

        m_socket = sock;

        // Set to non_blocking
        unsigned long non_blocking = 1;
        ioctlsocket( m_socket, FIONBIO, &non_blocking );

    }

    void ProcessIncoming()
    {
        // Try to recv data, read until nothing there
        // or it errored out
        unsigned char buffer[PACKET_MTU];

        sockaddr_storage their_addr;

        while (owner->IsRunning())
        {
            size_t addrlen = sizeof( their_addr );
            int recvd = recvfrom( m_socket, (char*)buffer, PACKET_MTU, 0, (sockaddr*)&their_addr, (int*) &addrlen );

            if (recvd > 0)
            {
                packetQueue->EnqueueRead( buffer, PACKET_MTU, (sockaddr*)&their_addr, addrlen );
            }
            else if (recvd < 0)
            {
                int error = WSAGetLastError();

                switch (error)
                {
                case WSAEWOULDBLOCK: // nothing to read
                case WSAEMSGSIZE: // was too large, truncated, throw it out
                    break;
                default:
                    owner->Close();
                    break;
                };
                return;
            }
            else
            {
                owner->Close();
            }
        }
    }

    void ProcessOutgoing()
    {
        NetworkPacket* packet = packetQueue->DequeueWrite();
        while (packet != nullptr )
        {
            NetworkAddress* addr = packet->GetAddress();
            sendto( m_socket, packet->GetBuffer(), packet->GetLength(), 0, (sockaddr*)&addr->addr, addr->addrlen );
            delete packet;

            packet = packetQueue->DequeueWrite()
        }
    }

    void ServiceSocket()
    {

        while (owner->IsRunning())
        {
            ProcessIncoming();
            ProcessOutgoing();

            yield();

        }
    }

protected:
    virtual void Run()
    {
        SetupSocket();
    }

public:
    void init( UDPSocket* owner, NetworkPacketQueue* queue, short port )
    {
        m_socket = INVALID_SOCKET;
        this->port = port;
        this->packetQueue = queue;
        this->owner = owner;
    }

};


#endif
