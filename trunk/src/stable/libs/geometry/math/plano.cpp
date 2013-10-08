#include "plano.h"

Plano::Plano(float A, float B, float C, float D)
{
    this->A = A;
    this->B = B;
    this->C = C;
    this->D = D;
}

//http://paulbourke.net/geometry/pointlineplane/
Plano::Plano(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d p3)
{
    this->A = p1(1)*(p2(2) - p3(2)) + p2(1)*(p3(2) - p1(2)) + p3(1)*(p1(2) - p2(2));

    this->B = p1(2)*(p2(0) - p3(0)) + p2(2)*(p3(0) - p1(0)) + p3(2)*(p1(0) - p2(0));

    this->C = p1(0)*(p2(1) - p3(1)) + p2(0)*(p3(1) - p1(1)) + p3(0)*(p1(1) - p2(1));

    this->D = -(p1(0)*(p2(1)*p3(2) - p3(1)*p2(2)) + p2(0)*(p3(1)*p1(2) - p1(1)*p3(2)) + p3(0)*(p1(1)*p2(2) - p2(1)*p1(2)));

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

float Plano::distanciaAPunto(Eigen::Vector3d p)
{
    float numerador = A*p(0) + B*p(1) + C*p(2) + D;
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

Eigen::Vector3d Plano::InterConRecta(Eigen::Vector3d p, Eigen::Vector3d q)
{
    Eigen::Vector3d result(0,0,0);

    float u = calculateU(A, B, C, D, p(0), p(1), p(2), q(0), q(1), q(2)) ;
    //std::cout << "U: " << u << std::endl;
    if( u <= 1 && u >= 0){
        result(0) = p(0) + u*(q(0)-p(0));
        result(1) = p(1) + u*(q(1)-p(1));
        result(2) = p(2) + u*(q(2)-p(2));
        //std::cout << "X: " << X  << " Y: " << Y << " Z: " << Z << std::endl;
    }
    return result;
}

Eigen::Vector3d Plano::InterConRecta(float px, float py, float pz, float qx, float qy, float qz)
{
    Eigen::Vector3d result(0,0,0);

    float u = calculateU(A, B, C, D, px, py, pz, qx, qy, qz) ;
    //std::cout << "U: " << u << std::endl;
    if( u <= 1 && u >= 0){
        result(0) = px + u*(qx-px);
        result(1) = py + u*(qy-py);
        result(2) = pz + u*(qz-pz);
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
Eigen::Vector3d Plano::proyeccionOrtogonal(float px, float py, float pz, float vx, float vy, float vz)
{
    float t = (-A*px - B*py - C*pz - D )/(A*vx + B*vy + C*vz );
    Eigen::Vector3d result(px+vx*t, py+vy*t, pz+vz*t);
    return result;
}

Eigen::Vector3d Plano::proyeccionOrtogonal(Eigen::Vector3d p, float vx, float vy, float vz)
{
    float t = (-A*p(0) - B*p(1) - C*p(2) - D )/(A*vx + B*vy + C*vz );
    Eigen::Vector3d result(p(0)+vx*t, p(1)+vy*t, p(2)+vz*t);
    return result;
}

