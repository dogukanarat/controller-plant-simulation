#include "system.h"
#include "osal.h"
#include <iostream>

using namespace Control;

void DynamicSystem::Define(struct SystemDefinition system)
{
    m_a << system.a1, system.a2, 0, 0;
    m_b << system.b1, system.b2, 1, 0;
    m_c << 0, 0, 0, 1;
    m_d << 1, 0;
    m_x << 0, 0;
    m_y << 0, 0;
}

void DynamicSystem::Iterate(Decimal input, Decimal &output)
{
    Eigen::Matrix<Decimal, 2, 1> temp;

    temp = m_c * m_x;

    m_x = temp + (m_d * input);

    temp = m_b * m_y;

    m_y = temp + m_a * m_x;

    output = m_y(1, 0);
}