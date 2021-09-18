#ifndef _NEEDMON_ETHERNET_H
#define _NEEDMON_ETHERNET_H

#include "stdint.h"
#include "os_socket.h"
#include "buffer.h"
#include "osal.h"
#include "communication.h"

namespace Needmon
{
    typedef enum
    {
        ETHERNET_UDP,
        ETHERNET_TCP,
    } EthernetProtocol;

    class Ethernet
    {
        public:
        Ethernet(const char* ipAddress, int portNo);

        uint16_t m_portNo;
        char* m_ipAddress;
        int m_serverSocket;
        EthernetProtocol m_protocol;
        OSAL::Socket::host_t* m_host;
        OSAL::Socket::address_in_t* m_serverSocketAddress;
        OSAL::Socket::length_t m_serverLen;
        
    };

    class TCP : public Ethernet
    {
        public:
        TCP(const char * ipAddress, int portNo);
        ~TCP() {};
    };

    class UDP : public Ethernet
    {
        public:
        UDP(const char * ipAddress, int portNo);
        ~UDP() {};
    };
}
#endif