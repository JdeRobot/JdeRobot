#include "vector3.h"
namespace math
{
    Vector3::Vector3()
    {
        vector<< 0.0, 0.0, 0.0;
    }
    Vector3::Vector3(const double &_x, const double &_y, const double &_z)
    {
        vector<< _x, _y, _z;

    }

    float Vector3::getX()
    {
        return vector(0);
    }

    float Vector3::getY()
    {
        return vector(1);
    }
    float Vector3::getZ()
    {
        return vector(2);
    }

    void Vector3::setX(float f)
    {
        vector(0) = f;
    }

    void Vector3::setY(float f)
    {
        vector(1) = f;
    }

    void Vector3::setZ(float f)
    {
        vector(2) = f;
    }

    //////////////////////////////////////////////////
    Vector3 Vector3::normalize()
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
    Vector3 Vector3::operator+(const Vector3 &pt) const
    {
        return Vector3(this->vector(0) + pt.vector(0), this->vector(1) + pt.vector(1), this->vector(2) + pt.vector(2));
    }

    Vector3 Vector3::operator-(const Vector3 &pt) const
    {
        return Vector3(this->vector(0) - pt.vector(0), this->vector(1) - pt.vector(1), this->vector(2) - pt.vector(2));
    }

    //////////////////////////////////////////////////
    Vector3 Vector3::operator*(double v) const
    {
      return Vector3(this->vector(0) * v, this->vector(1) * v, this->vector(2) * v);
    }

    //////////////////////////////////////////////////
    const Vector3 &Vector3::operator+=(const Vector3 &pt)
    {
        this->vector(0) += pt.vector(0);
        this->vector(1) += pt.vector(1);
        this->vector(2) += pt.vector(2);

      return *this;
    }

    double Vector3::distance(math::Vector3 p){
        return sqrt( pow(p.getX()-this->vector(0),2) +
                     pow(p.getY()-this->vector(1),2) +
                     pow(p.getZ()-this->vector(2),2));
    }


}
