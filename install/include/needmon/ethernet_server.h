#ifndef _NEEDMON_ETHERNET_SERVER_H
#define _NEEDMON_ETHERNET_SERVER_H

#include "stdint.h"
#include "osal.h"
#include "buffer.h"
#include "communication.h"
#include "ethernet.h"

namespace Needmon
{
    class Server : public Communication
    {
        public:
        Server(Ethernet* ethernet);
        ~Server();

        OSAL::Socket::address_in_t* m_clientSocketAddress = nullptr;
        OSAL::Socket::length_t m_clientSocketLen;
        int m_clientSocket;
        Ethernet* m_ethernet;

        ErrorNo Connect() override;
        ErrorNo Process() override;
        ErrorNo Read(Buffer& buffer) override;
        ErrorNo Write(Buffer& buffer) override;
    };
}

#endif