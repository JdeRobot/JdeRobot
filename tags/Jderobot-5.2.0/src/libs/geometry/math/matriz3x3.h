#ifndef MATRIZ3X3_H
#define MATRIZ3X3_H

#define EIGEN_DONT_ALIGN_STATICALLY True

#include <eigen3/Eigen/Dense>

#include "vector3.h"

namespace math
{
    class Matriz3x3
    {
    public: Matriz3x3();

    public: Eigen::Matrix3f matriz;

    public: Eigen::Matrix3f& getMatriz();

    public: Eigen::Matrix3f getCopyMatriz();

    public:
        void setFromAxis(float x, float y, float z, float angle);
        Matriz3x3 operator*(const Matriz3x3 &_m) const;

    public: friend std::ostream &operator<<(std::ostream &_out,
                                             Matriz3x3 &_m)
    {
            for (int i = 0; i < 3; i++)
            {
              for (int j = 0; j < 3; j++)
              {
                _out << _m.getMatriz()(i,j) << " ";
              }
              _out << "\n";
            }

            return _out;
    }
    };
}

#endif // MATRIZ3X3_H
