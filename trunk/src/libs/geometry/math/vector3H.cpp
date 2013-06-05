#include "vector3H.h"
namespace math
{
    Vector3H::Vector3H()
    {
        vector<< 0.0, 0.0, 0.0, 0.0;
    }
    Vector3H::Vector3H(const double &_x, const double &_y, const double &_z, const double &_h)
    {
        vector<< _x, _y, _z, _h;

    }

    float Vector3H::getX()
    {
        return vector(0);
    }

    float Vector3H::getY()
    {
        return vector(1);
    }
    float Vector3H::getZ()
    {
        return vector(2);
    }
    
    float Vector3H::getH()
    {
        return vector(3);
    }

    void Vector3H::setX(float f)
    {
        vector(0) = f;
    }

    void Vector3H::setY(float f)
    {
        vector(1) = f;
    }

    void Vector3H::setZ(float f)
    {
        vector(2) = f;
    }
    
    void Vector3H::setH(float f)
    {
        vector(3) = f;
    }

    //////////////////////////////////////////////////
    Vector3H Vector3H::normalize()
    {
        double d = sqrt(this->getX() * this->getX() + this->getY() * this->getY() + this->getZ() * this->getZ());

      if (!math::equal(d, 0.0))
      {
          this->setX(this->getX()/ d);
          this->setY(this->getY()/ d);
          this->setZ(this->getZ()/ d);
      }

      return *this;
    }

    //////////////////////////////////////////////////
    Vector3H Vector3H::operator+(const Vector3H &pt) const
    {
        return Vector3H(this->vector(0) + pt.vector(0), this->vector(1) + pt.vector(1), this->vector(2) + pt.vector(2));
    }

    Vector3H Vector3H::operator-(const Vector3H &pt) const
    {
        return Vector3H(this->vector(0) - pt.vector(0), this->vector(1) - pt.vector(1), this->vector(2) - pt.vector(2));
    }

    //////////////////////////////////////////////////
    Vector3H Vector3H::operator*(double v) const
    {
      return Vector3H(this->vector(0) * v, this->vector(1) * v, this->vector(2) * v);
    }

    //////////////////////////////////////////////////
    const Vector3H &Vector3H::operator+=(const Vector3H &pt)
    {
        this->vector(0) += pt.vector(0);
        this->vector(1) += pt.vector(1);
        this->vector(2) += pt.vector(2);

      return *this;
    }


}
