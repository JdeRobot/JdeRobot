#ifndef VECTOR2D_H
#define VECTOR2D_H

#define EIGEN_DONT_ALIGN_STATICALLY True

#include <eigen3/Eigen/Dense>

namespace math
{
    class Vector2H
    {
    public:
        Vector2H();
        Vector2H(float _x, float _y, float _h=1.0);

    public: float getX();
    public: float getY();
    public: float getH();

    public: float getX() const;
    public: float getY() const;

    public: void setX(float _x);
    public: void setY(float _y);
    public: void setH(float _h);

    public:     double mag_squared() const;
    public:     double mag_squared() ;


    //OPERATORS
    public:    Vector2H operator-(const Vector2H &pt) const;
    public:    const Vector2H &operator-=(const Vector2H &pt);

    public:    const Vector2H &operator/=(double v);
    public:    const Vector2H operator/(double v) const;
    public:    const Vector2H &operator/=(const Vector2H &pt);
    public:    const Vector2H operator/(const Vector2H &pt) const;

    const Vector2H operator*(const Vector2H &pt) const;
    const Vector2H &operator*=(const Vector2H &pt);
    const Vector2H operator*(double v) const;
    const Vector2H &operator*=(double v);


    const Vector2H &operator =(double value);
    Vector2H &operator =(const Vector2H &pt);

    const Vector2H &operator+=(const Vector2H &pt);
    Vector2H operator+(const Vector2H &pt) const;


    public: Eigen::Vector3f vector;
    };
}

#endif // VECTOR2D_H
