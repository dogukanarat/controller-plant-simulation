#ifndef _NEEDMON_ETHERNET_CLIENT_H
#define _NEEDMON_ETHERNET_CLIENT_H

#include "stdint.h"
#include "osal.h"
#include "buffer.h"
#include "communication.h"
#include "ethernet.h"

namespace Needmon
{
    class Client : public Communication
    {
        public:
        Client(Ethernet* ethernet);
        ~Client() {};

        Ethernet* m_ethernet;

        ErrorNo Connect() override;
        ErrorNo Process() override;
        ErrorNo Read(Buffer& buffer) override;
        ErrorNo Write(Buffer& buffer) override;
    };

}

#endif