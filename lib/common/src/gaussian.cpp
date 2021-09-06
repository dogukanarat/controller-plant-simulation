#include "gaussian.h"
#include <stdlib.h> 
#include <time.h>
#include <math.h>

using namespace Control;

#ifndef M_PI
#define M_PI 3.14159265
#endif

void GaussianDistribution::Update(Decimal mean, Decimal variance)
{
    m_mean = ( (mean * m_variance + m_mean * variance) / (variance + m_variance) );
    m_variance = ( ( variance * m_variance ) / ( variance + m_variance ) );
}

void GaussianDistribution::Reset(Decimal mean, Decimal variance)
{
    m_mean = mean;
    m_variance = variance;
}

Decimal GaussianDistribution::Random()
{
    srand( (unsigned)time( NULL ) );

    Decimal R1 = (Decimal)rand()/(Decimal)RAND_MAX;
    Decimal R2 = (Decimal)rand()/(Decimal)RAND_MAX;

    return m_mean + m_variance * cos( 2 * M_PI * R1) * sqrt(-log(R2) * 2);
}

Decimal GaussianDistribution::Random(Decimal actual)
{
    m_mean = actual;
    return Random();
}

Decimal GaussianDistribution::GetMaxima()
{
    return m_mean;
}

