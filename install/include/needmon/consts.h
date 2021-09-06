#ifndef __NEEDMON_CONSTS_H
#define __NEEDMON_CONSTS_H

namespace Needmon
{
    typedef float  float32_t;
    typedef double float64_t;

    typedef enum
    {
        FRAME_HEADER_IDX_SYNC_HIGH    = 0,
        FRAME_HEADER_IDX_SYNC_LOW        ,
        FRAME_HEADER_IDX_MESSAGE_ID      ,
        FRAME_HEADER_IDX_MESSAGE_SIZE    ,
        FRAME_HEADER_SIZE                ,
    } FrameHeaderIndex;

    typedef enum
    {
        MAX_PAYLOAD_SIZE  = 255,
    } PayloadConsts;
    
    typedef enum
    {
        FRAME_SIZE         = MAX_PAYLOAD_SIZE + FRAME_HEADER_SIZE,
        FRAME_SYNC_HIGH    = 0xAB,
        FRAME_SYNC_LOW     = 0xCD,

    } FrameConsts;

}

#endif