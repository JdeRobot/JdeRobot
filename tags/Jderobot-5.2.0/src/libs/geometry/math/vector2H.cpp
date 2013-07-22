#include "vector2H.h"
namespace math
{
    Vector2H::Vector2H()
    {
        vector << 0, 0, 0;

    }

    Vector2H::Vector2H(float _x, float _y, float _h)
    {
        vector << _x, _y, _h;

    }

    float Vector2H::getX()
    {
        return vector(0);
    }

    float Vector2H::getY()
    {
        return vector(1);
    }

    float Vector2H::getH()
    {
        return vector(2);
    }

    float Vector2H::getX() const
    {
        return vector(0);
    }

    float Vector2H::getY() const
    {
        return vector(1);
    }

    void Vector2H::setX(float _x)
    {
        vector(0) = _x;
    }

    void Vector2H::setY(float _y )
    {
        vector(1) = _y;

    }

    void Vector2H::setH(float _h )
    {
        vector(2) = _h;

    }

    //////////////////////////////////////////////////
    Vector2H Vector2H::operator-(const Vector2H &pt) const
    {
      return Vector2H(vector(0) - pt.vector(0), vector(1) - pt.vector(1));
    }

    const Vector2H &Vector2H::operator-=(const Vector2H &pt)
    {
      vector(0) -= pt.vector(0);
      vector(1) -= pt.vector(1);

      return *this;
    }

    //////////////////////////////////////////////////

    const Vector2H Vector2H::operator/(const Vector2H &pt) const
    {
      return Vector2H(vector(0) / pt.vector(0), vector(1) / pt.vector(1));
    }

    const Vector2H &Vector2H::operator/=(const Vector2H &pt)
    {
      vector(0) /= pt.vector(0);
      vector(1) /= pt.vector(1);

      return *this;
    }

    const Vector2H Vector2H::operator/(double v) const
    {
      return Vector2H(vector(0) / v, vector(1) / v);
    }

    const Vector2H &Vector2H::operator/=(double v)
    {
      vector(0) /= v;
      vector(1) /= v;

      return *this;
    }

    double Vector2H::mag_squared() const
    {
        return (vector(0)*vector(0))*(vector(1)*vector(1));
    }

    double Vector2H::mag_squared()
    {
        return (vector(0)*vector(0))*(vector(1)*vector(1));
    }


    //////////////////////////////////////////////////
    const Vector2H Vector2H::operator*(const Vector2H &pt) const
    {
      return Vector2H(vector(0) * pt.vector(0), vector(1) * pt.vector(1));
    }

    const Vector2H &Vector2H::operator*=(const Vector2H &pt)
    {
      vector(0) *= pt.vector(0);
      vector(1) *= pt.vector(1);

      return *this;
    }

    const Vector2H Vector2H::operator*(double v) const
    {
      return Vector2H(vector(0) * v, vector(1) * v);
    }

    const Vector2H &Vector2H::operator*=(double v)
    {
      vector(0) *= v;
      vector(1) *= v;

      return *this;
    }

    //////////////////////////////////////////////////
    Vector2H &Vector2H::operator =(const Vector2H &pt)
    {
      vector(0) = pt.vector(0);
      vector(1) = pt.vector(1);

      return *this;
    }

    //////////////////////////////////////////////////
    const Vector2H &Vector2H::operator =(double value)
    {
      vector(0) = value;
      vector(1) = value;

      return *this;
    }

    //////////////////////////////////////////////////
    Vector2H Vector2H::operator+(const Vector2H &pt) const
    {
      return Vector2H(vector(0) + pt.vector(0), vector(1) + pt.vector(1));
    }

    const Vector2H &Vector2H::operator+=(const Vector2H &pt)
    {
      vector(0) += pt.vector(0);
      vector(1) += pt.vector(1);

      return *this;
    }


}
