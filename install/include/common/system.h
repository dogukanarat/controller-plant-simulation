#include "stdint.h"
#include <Eigen/Dense>
#include "gaussian.h"

namespace Control
{
    struct SystemDefinition
    {
        Decimal a1;
        Decimal a2;
        Decimal b1;
        Decimal b2;
    };

    class DynamicSystem
    {
        public:
        DynamicSystem() {};
        ~DynamicSystem() {};
        
        Eigen::Matrix2f m_a;
        Eigen::Matrix2f m_b;
        Eigen::Matrix2f m_c;
        Eigen::Matrix<Decimal,2,1> m_d;
        Eigen::Matrix<Decimal,2,1> m_y;
        Eigen::Matrix<Decimal,2,1> m_x;

        void Define(struct SystemDefinition system);
        void Iterate(Decimal input, Decimal& output);
        

    };
}