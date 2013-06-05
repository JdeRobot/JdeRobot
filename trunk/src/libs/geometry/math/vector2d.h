#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <eigen3/Eigen/Dense>

namespace math
{
    class Vector2d
    {
    public:
        Vector2d();
        Vector2d(float _x, float _y);

    public: float getX();
    public: float getY();

    public: float getX() const;
    public: float getY() const;

        public: void setX(float _x);
        public: void setY(float _y);

    public:     double mag_squared() const;
    public:     double mag_squared() ;


        //OPERATORS
        public:    Vector2d operator-(const Vector2d &pt) const;
        public:    const Vector2d &operator-=(const Vector2d &pt);

        public:    const Vector2d &operator/=(double v);
        public:    const Vector2d operator/(double v) const;
        public:    const Vector2d &operator/=(const Vector2d &pt);
        public:    const Vector2d operator/(const Vector2d &pt) const;

        const Vector2d operator*(const Vector2d &pt) const;
        const Vector2d &operator*=(const Vector2d &pt);
        const Vector2d operator*(double v) const;
        const Vector2d &operator*=(double v);


        const Vector2d &operator =(double value);
        Vector2d &operator =(const Vector2d &pt);

        const Vector2d &operator+=(const Vector2d &pt);
        Vector2d operator+(const Vector2d &pt) const;


    public: Eigen::Vector2f vector;
    };
}

#endif // VECTOR2D_H
