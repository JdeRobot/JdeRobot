#ifndef VECTOR3_H
#define VECTOR3_H

#include <eigen3/Eigen/Dense>

#include "matriz3x3.h"

#include "utils.h"

namespace math
{
    class Vector3
    {
        public: Vector3();
        public: Vector3(const double &_x, const double &_y, const double &_z);

        public: float getX();
        public: float getY();
        public: float getZ();

        public: void setX(float f);
        public: void setY(float f);
        public: void setZ(float f);

        Vector3 normalize();
        double distance(math::Vector3 p);

        public: Eigen::Vector3f vector;

    public: Vector3 operator+(const Vector3 &pt) const;
            Vector3 operator-(const Vector3 &pt) const;
            Vector3 operator*(double v) const;
            const Vector3 &operator+=(const Vector3 &pt);


    public: friend std::ostream &operator<<(std::ostream &_out,
                                            Vector3 &_pt)
    {
            _out << math::precision(_pt.getX(), 6) << " " << math::precision(_pt.getY(), 6) << " "
            << math::precision(_pt.getZ(), 6);
            return _out;
    }

    public: friend std::istream &operator>>(std::istream &_in,
                                            Vector3 &_pt)
    {
      // Skip white spaces
      _in.setf(std::ios_base::skipws);

      float x, y, z;

      _in >> x >> y >> z;
      _pt.setX(x);
      _pt.setY(y);
      _pt.setZ(z);
      return _in;
    }



    };
}

#endif // VECTOR3_H
