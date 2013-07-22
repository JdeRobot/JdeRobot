#include "plano.h"

Plano::Plano(float A, float B, float C, float D)
{
    this->A = A;
    this->B = B;
    this->C = C;
    this->D = D;
}

//http://paulbourke.net/geometry/pointlineplane/
Plano::Plano(math::Vector3 p1, math::Vector3 p2, math::Vector3 p3)
{
    this->A = p1.getY()*(p2.getZ() - p3.getZ()) + p2.getY()*(p3.getZ() - p1.getZ()) + p3.getY()*(p1.getZ() - p2.getZ());

    this->B = p1.getZ()*(p2.getX() - p3.getX()) + p2.getZ()*(p3.getX() - p1.getX()) + p3.getZ()*(p1.getX() - p2.getX());

    this->C = p1.getX()*(p2.getY() - p3.getY()) + p2.getX()*(p3.getY() - p1.getY()) + p3.getX()*(p1.getY() - p2.getY());

    this->D = -(p1.getX()*(p2.getY()*p3.getZ() - p3.getY()*p2.getZ()) + p2.getX()*(p3.getY()*p1.getZ() - p1.getY()*p3.getZ()) + p3.getX()*(p1.getY()*p2.getZ() - p2.getY()*p1.getZ()));

}


float Plano::getCoefA()
{
    return A;
}

float Plano::getCoefB()
{
    return B;
}

float Plano::getCoefC()
{
    return C;
}

float Plano::getCoefD()
{
    return D;
}

void Plano::setCoefA(float f)
{
    this->A = f;
}

void Plano::setCoefB(float f)
{
    this->B = f;
}

void Plano::setCoefC(float f)
{
    this->C = f;
}

void Plano::setCoefD(float f)
{
    this->D = f;
}

float Plano::distanciaAPunto(float x, float y, float z)
{
    float numerador = A*x + B*y + C*z + D;
    float denominador = sqrt( pow(A, 2) + pow(B, 2) + pow(C, 2));

    if(denominador!=0)
        return numerador/denominador;
    return 10000000;
}

float Plano::distanciaAPunto(math::Vector3 p)
{
    float numerador = A*p.getX() + B*p.getY() + C*p.getZ() + D;
    float denominador = sqrt( pow(A, 2) + pow(B, 2) + pow(C, 2));

    if(denominador!=0)
        return numerador/denominador;
    return 10000000;
}

float Plano::calculateU(float A, float B, float C, float D, float px, float py, float pz, float qx, float qy, float qz){
    if( (A* (px-qx) + B* (py - qy) + C *( pz - qz ) != 0 ) )
       return ( A * px + B * py + C * pz + D ) / (A* (px-qx) + B* (py - qy) + C *( pz - qz ) ) ;
    return -1;
}

math::Vector3 Plano::InterConRecta(math::Vector3 p, math::Vector3 q)
{
    math::Vector3 result(0,0,0);

    float u = calculateU(A, B, C, D, p.getX(), p.getY(), p.getZ(), q.getX(), q.getY(), q.getZ()) ;
    //std::cout << "U: " << u << std::endl;
    if( u <= 1 && u >= 0){
        result.setX(p.getX() + u*(q.getX()-p.getX()));
        result.setY(p.getY() + u*(q.getY()-p.getY()));
        result.setZ(p.getZ() + u*(q.getZ()-p.getZ()));
        //std::cout << "X: " << X  << " Y: " << Y << " Z: " << Z << std::endl;
    }
    return result;
}

math::Vector3 Plano::InterConRecta(float px, float py, float pz, float qx, float qy, float qz)
{
    math::Vector3 result(0,0,0);

    float u = calculateU(A, B, C, D, px, py, pz, qx, qy, qz) ;
    //std::cout << "U: " << u << std::endl;
    if( u <= 1 && u >= 0){
        result.setX(px + u*(qx-px));
        result.setY(py + u*(qy-py));
        result.setZ(pz + u*(qz-pz));
        //std::cout << "X: " << X  << " Y: " << Y << " Z: " << Z << std::endl;
    }
    return result;
}

//parametricas
// x = px + vx*t
// y = py + vy*t
// z = pz + vz*t

// A* (px + vx*t) + B(py + vy*t) +C(pz+vz*t) + D = 0;
// (-A*px - D - B*py - C*pz) = (A *vx *t + B *vy *t + C *vz *t)
// t = (-A*px - D - B*py - C*pz)/ (A*vx + B*vy + C* vz)
math::Vector3 Plano::proyeccionOrtogonal(float px, float py, float pz, float vx, float vy, float vz)
{
    float t = (-A*px - B*py - C*pz - D )/(A*vx + B*vy + C*vz );
    math::Vector3 result(px+vx*t, py+vy*t, pz+vz*t);
    return result;
}

math::Vector3 Plano::proyeccionOrtogonal(math::Vector3 p, float vx, float vy, float vz)
{
    float t = (-A*p.getX() - B*p.getY() - C*p.getZ() - D )/(A*vx + B*vy + C*vz );
    math::Vector3 result(p.getX()+vx*t, p.getY()+vy*t, p.getZ()+vz*t);
    return result;
}

