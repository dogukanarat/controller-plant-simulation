#ifndef _NEEDMON_PAYLOAD_HPP
#define _NEEDMON_PAYLOAD_HPP

#include <stdint.h>
#include <stdlib.h>
#include <typeinfo>
#include "consts.h"

namespace Needmon
{
    typedef union
    {
        uint8_t    ucData      ;
        uint16_t   usData      ;
        uint32_t   ulData      ;
        uint64_t   ullData     ;
        float32_t  fData       ;
        float64_t  dData       ;
        int8_t     cData       ;
        int16_t    sData       ;
        int32_t    lData       ;
        int64_t    llData      ;
        uint8_t    ucBytes[8]  ;
    } byteConnector_t;

    class Payload
    {
        public:
        Payload()
        : m_writeIndex(0)
        , m_readIndex(0)
        {};
        ~Payload() {};

        void Write( uint8_t   &data );
        void Write( uint16_t  &data );
        void Write( uint32_t  &data );
        void Write( uint64_t  &data );
        void Write( int8_t    &data );
        void Write( int16_t   &data );
        void Write( int32_t   &data );
        void Write( int64_t   &data );
        void Write( float32_t &data );
        void Write( float64_t &data );

        void Read( uint8_t   &returnBuffer );
        void Read( uint16_t  &returnBuffer );
        void Read( uint32_t  &returnBuffer );
        void Read( uint64_t  &returnBuffer );
        void Read( int8_t    &returnBuffer );
        void Read( int16_t   &returnBuffer );
        void Read( int32_t   &returnBuffer );
        void Read( int64_t   &returnBuffer );
        void Read( float32_t &returnBuffer );
        void Read( float64_t &returnBuffer );

        protected:
        uint8_t m_writeIndex;
        uint8_t m_readIndex;
        uint8_t m_dataCount;
        uint8_t  m_payloadBuffer[MAX_PAYLOAD_SIZE];

        void GetDataCount( uint8_t& dataCount );
        void SetDataCount( uint8_t& dataCount );
        void ResetBuffer();     
        void WriteToBuffer( byteConnector_t &converter, uint8_t byteCount );
        void ReadFromBuffer( byteConnector_t &converter, uint8_t byteCount );
    };
}

#endif