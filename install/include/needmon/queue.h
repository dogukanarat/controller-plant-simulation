#include "frame.h"
#include "stdint.h"
#include <list>
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

        std::list<Frame> m_instances;

        Queue& Pop(std::shared_ptr<Frame> frame);
        Queue& Push(std::shared_ptr<Frame> frame);
    };
}