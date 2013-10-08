#ifndef PLANO_H
#define PLANO_H

#include <math.h>
#include <eigen3/Eigen/Dense>

class Plano
{
public:
    Plano(float A, float B, float C, float D);
    Plano(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d p3);

    float getCoefA();
    float getCoefB();
    float getCoefC();
    float getCoefD();

    void setCoefA(float f);
    void setCoefB(float f);
    void setCoefC(float f);
    void setCoefD(float f);

    Eigen::Vector3d InterConRecta(Eigen::Vector3d p, Eigen::Vector3d q);
    Eigen::Vector3d InterConRecta(float px, float py, float pz, float qx, float qy, float qz);
    float calculateU(float A, float B, float C, float D, float px, float py, float pz, float qx, float qy, float qz);
    float distanciaAPunto(float x, float y, float z);
    float distanciaAPunto(Eigen::Vector3d p);

    Eigen::Vector3d proyeccionOrtogonal(float px, float py, float pz, float vx, float vy, float vz);
    Eigen::Vector3d proyeccionOrtogonal(Eigen::Vector3d p, float vx, float vy, float vz);

private:
    float A;
    float B;
    float C;
    float D;

};

#endif // PLANO_H
