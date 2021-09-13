#ifndef _NEEDMON_FRAME_HPP
#define _NEEDMON_FRAME_HPP

#include "consts.h"
#include "payload.h"
#include "buffer.h"
#include "packet.h"

namespace Needmon
{
    class Frame
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

        void Decode(Packet& packet);
        void Encode(Packet& packet);

        friend Frame operator << (Packet&,Frame&);

        void EncodeReset();
        void DecodeReset();

        Payload m_payload;
        
        private:
        uint8_t m_messageId  ;
        uint8_t m_messageSize;
        uint8_t m_syncHigh;
        uint8_t m_syncLow;

    };
}

#endif