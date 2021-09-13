#include "frame.h"
#include "stdint.h"
#include <queue>
#include <memory>

namespace Needmon
{
    class Queue
    {
        public:
        Queue(uint16_t maxLength);
        ~Queue() {};

        uint16_t m_maxLength;
        uint16_t m_length;
        uint8_t m_head;
        uint8_t m_tail;
        uint8_t m_isFailed;

        std::shared_ptr<Frame>* m_frames;

        Queue& Pop(std::shared_ptr<Frame> frame);
        Queue& Push(std::shared_ptr<Frame> frame);
        uint16_t Length();
        uint8_t IsFailed();
    };
}