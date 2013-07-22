#include "vector2d.h"
namespace math
{
    Vector2d::Vector2d()
    {
        vector << 0, 0;

    }

    Vector2d::Vector2d(float _x, float _y)
    {
        vector << _x, _y;

    }

    float Vector2d::getX()
    {
        return vector(0);
    }

    float Vector2d::getY()
    {
        return vector(1);
    }

    float Vector2d::getX() const
    {
        return vector(0);
    }

    float Vector2d::getY() const
    {
        return vector(1);
    }

    void Vector2d::setX(float _x)
    {
        vector(0) = _x;
    }

    void Vector2d::setY(float _y )
    {
        vector(1) = _y;

    }

    //////////////////////////////////////////////////
    Vector2d Vector2d::operator-(const Vector2d &pt) const
    {
      return Vector2d(vector(0) - pt.vector(0), vector(1) - pt.vector(1));
    }

    const Vector2d &Vector2d::operator-=(const Vector2d &pt)
    {
      vector(0) -= pt.vector(0);
      vector(1) -= pt.vector(1);

      return *this;
    }

    //////////////////////////////////////////////////

    const Vector2d Vector2d::operator/(const Vector2d &pt) const
    {
      return Vector2d(vector(0) / pt.vector(0), vector(1) / pt.vector(1));
    }

    const Vector2d &Vector2d::operator/=(const Vector2d &pt)
    {
      vector(0) /= pt.vector(0);
      vector(1) /= pt.vector(1);

      return *this;
    }

    const Vector2d Vector2d::operator/(double v) const
    {
      return Vector2d(vector(0) / v, vector(1) / v);
    }

    const Vector2d &Vector2d::operator/=(double v)
    {
      vector(0) /= v;
      vector(1) /= v;

      return *this;
    }

    double Vector2d::mag_squared() const
    {
        return (vector(0)*vector(0))*(vector(1)*vector(1));
    }

    double Vector2d::mag_squared()
    {
        return (vector(0)*vector(0))*(vector(1)*vector(1));
    }


    //////////////////////////////////////////////////
    const Vector2d Vector2d::operator*(const Vector2d &pt) const
    {
      return Vector2d(vector(0) * pt.vector(0), vector(1) * pt.vector(1));
    }

    const Vector2d &Vector2d::operator*=(const Vector2d &pt)
    {
      vector(0) *= pt.vector(0);
      vector(1) *= pt.vector(1);

      return *this;
    }

    const Vector2d Vector2d::operator*(double v) const
    {
      return Vector2d(vector(0) * v, vector(1) * v);
    }

    const Vector2d &Vector2d::operator*=(double v)
    {
      vector(0) *= v;
      vector(1) *= v;

      return *this;
    }

    //////////////////////////////////////////////////
    Vector2d &Vector2d::operator =(const Vector2d &pt)
    {
      vector(0) = pt.vector(0);
      vector(1) = pt.vector(1);

      return *this;
    }

    //////////////////////////////////////////////////
    const Vector2d &Vector2d::operator =(double value)
    {
      vector(0) = value;
      vector(1) = value;

      return *this;
    }

    //////////////////////////////////////////////////
    Vector2d Vector2d::operator+(const Vector2d &pt) const
    {
      return Vector2d(vector(0) + pt.vector(0), vector(1) + pt.vector(1));
    }

    const Vector2d &Vector2d::operator+=(const Vector2d &pt)
    {
      vector(0) += pt.vector(0);
      vector(1) += pt.vector(1);

      return *this;
    }


}
