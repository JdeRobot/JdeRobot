#ifndef Vector3H_H
#define Vector3H_H

#define EIGEN_DONT_ALIGN_STATICALLY True

#include <eigen3/Eigen/Dense>

#include "matriz3x3.h"

#include "utils.h"

namespace math
{
    class Vector3H
    {
        public: Vector3H();
        public: Vector3H(const double &_x, const double &_y, const double &_z, const double &_h=1);

        public: float getX();
        public: float getY();
        public: float getZ();
        public: float getH();

        public: void setX(float f);
        public: void setY(float f);
        public: void setZ(float f);
        public: void setH(float f);

        Vector3H normalize();
        public: Eigen::Vector4f vector;

    public: Vector3H operator+(const Vector3H &pt) const;
            Vector3H operator-(const Vector3H &pt) const;
            Vector3H operator*(double v) const;
            const Vector3H &operator+=(const Vector3H &pt);


    public: friend std::ostream &operator<<(std::ostream &_out,
                                            Vector3H &_pt)
    {
            _out << math::precision(_pt.getX(), 6) << " " << math::precision(_pt.getY(), 6) << " "
            << math::precision(_pt.getZ(), 6) << " " << math::precision(_pt.getH(), 6);
            return _out;
    }

    public: friend std::istream &operator>>(std::istream &_in,
                                            Vector3H &_pt)
    {
      // Skip white spaces
      _in.setf(std::ios_base::skipws);

      float x, y, z, h;

      _in >> x >> y >> z >> h;
      _pt.setX(x);
      _pt.setY(y);
      _pt.setZ(z);
      _pt.setH(h);
      return _in;
    }



    };
}

#endif // Vector3H_H
