#ifndef _NEEDMON_COMMUNICATION_H
#define _NEEDMON_COMMUNICATION_H

#include "stdint.h"
#include "os_socket.h"
#include "buffer.h"
#include "os_stream.h"

namespace Needmon
{
    typedef int ErrorNo;
    
    class Communication
    {
        public:

        virtual ErrorNo SetHandler(void (*receiveHandler)(uint8_t*,uint32_t)) { m_receiveHandler = receiveHandler; return true; };
        virtual ErrorNo Connect() = 0;
        virtual ErrorNo Process() = 0;
        virtual ErrorNo Read(Buffer& buffer) = 0;
        virtual ErrorNo Write(Buffer& buffer) = 0;

        protected:
        Communication() {};
        virtual ~Communication() {};

        Buffer m_buffer;
        void (*m_receiveHandler)(uint8_t*,uint32_t) = nullptr;
    };

}

#endif