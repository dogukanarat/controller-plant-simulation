#ifndef _NEEDMON_BUFFER_H
#define _NEEDMON_BUFFER_H

#include "consts.h"
#include "stdint.h"

namespace Needmon
{
    const uint32_t BUFFER_SIZE = FRAME_SIZE;

    class Buffer
    {
        public:
        Buffer() {};
        ~Buffer() {};

        uint8_t Set(uint32_t index, uint8_t& value);
        uint8_t Get(uint32_t index, uint8_t& value);
        uint8_t* GetAddress();
        uint32_t GetSize();

        protected:
        uint32_t m_size = BUFFER_SIZE;
        uint8_t m_buffer[ BUFFER_SIZE ];
    };
}

#endif