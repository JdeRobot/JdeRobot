#ifndef PLANO_H
#define PLANO_H

#include <math.h>

#include "vector3.h"

class Plano
{
public:
    Plano(float A, float B, float C, float D);
    Plano(math::Vector3 p1, math::Vector3 p2, math::Vector3 p3);

    float getCoefA();
    float getCoefB();
    float getCoefC();
    float getCoefD();

    void setCoefA(float f);
    void setCoefB(float f);
    void setCoefC(float f);
    void setCoefD(float f);

    math::Vector3 InterConRecta(math::Vector3 p, math::Vector3 q);
    math::Vector3 InterConRecta(float px, float py, float pz, float qx, float qy, float qz);
    float calculateU(float A, float B, float C, float D, float px, float py, float pz, float qx, float qy, float qz);
    float distanciaAPunto(float x, float y, float z);
    float distanciaAPunto(math::Vector3 p);

    math::Vector3 proyeccionOrtogonal(float px, float py, float pz, float vx, float vy, float vz);
    math::Vector3 proyeccionOrtogonal(math::Vector3 p, float vx, float vy, float vz);

private:
    float A;
    float B;
    float C;
    float D;

};

#endif // PLANO_H
