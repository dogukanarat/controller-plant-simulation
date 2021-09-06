#ifndef _NEEDMON_FRAME_HPP
#define _NEEDMON_FRAME_HPP

#include "consts.h"
#include "payload.h"
#include "buffer.h"

namespace Needmon
{
    class Frame : public Payload
    {
        public:
        Frame()
        : m_syncHigh(FRAME_SYNC_HIGH)
        , m_syncLow(FRAME_SYNC_LOW)
        {};
        ~Frame() {};

        void SetMessageId( uint8_t& messageId );
        void GetMessageId( uint8_t& messageId );

        void Serialize( Buffer& buffer );
        void Parse( Buffer& buffer );
        void EncodeReset();
        void DecodeReset();
        
        private:
        uint8_t m_messageId  ;
        uint8_t m_messageSize;
        uint8_t m_syncHigh;
        uint8_t m_syncLow;

    };
}

#endif