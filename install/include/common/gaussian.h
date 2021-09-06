#include "stdint.h"

namespace Control
{
    typedef float Decimal;

    class GaussianDistribution
    {
        public:
        GaussianDistribution(Decimal mean, Decimal variance) 
        : m_mean{mean}
        , m_variance{variance} {};
        ~GaussianDistribution() {};

        Decimal m_mean;
        Decimal m_variance;

        void Update(Decimal mean, Decimal variance);
        void Reset(Decimal mean, Decimal variance);
        Decimal Random();
        Decimal Random(Decimal actual);
        Decimal GetMaxima();

    };
}