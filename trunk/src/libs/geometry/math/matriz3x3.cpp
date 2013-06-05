#include "matriz3x3.h"

namespace math
{
    Matriz3x3::Matriz3x3()
    {
        matriz   << 1, 0, 0,
                    0, 1, 0,
                    0, 0, 1;
    }

    Eigen::Matrix3f Matriz3x3::getMatriz()
    {
        return this->matriz;
    }

    Matriz3x3 Matriz3x3::operator*(const Matriz3x3 &_m) const
    {
        Matriz3x3 m;
        m.matriz = this->matriz*_m.matriz;
        return m;
    }

//    //////////////////////////////////////////////////
    void Matriz3x3::setFromAxis(float x, float y, float z, float angle)
    {
        if(x>0.1){

            matriz << 1., 0.,                    0.,
                        0., cos(angle), -sin(angle),
                        0., sin(angle), cos(angle);

        }

        if(y>0.1){
            matriz << cos(angle ),   0.,     sin(angle),
                        0.,                     1.0,    0.,
                        -sin(angle),   0.0,    cos(angle );
        }

        if(z>0.1){
            matriz << cos(angle),   -sin(angle),    0.,
                        sin(angle),    cos(angle),    0.,
                        0.,                     0.0,                    1.0;
        }

//        std::cout << matriz << std::endl;

//      float c = cos(_angle);
//      float s = sin(_angle);
//      float C = 1-c;

//      matriz(0,0) = x*x*C + c;
//      matriz(0,1) = x*y*C - z*s;
//      matriz(0,2) = x*z*C + y*s;

//      matriz(1,0) = y*x*C + z*s;
//      matriz(1,1) = y*y*C + c;
//      matriz(1,2) = y*z*C - x*s;

//      matriz(2,0) = z*x*C - y*s;
//      matriz(2,1) = z*y*C + x*s;
//      matriz(2,2) = z*z*C + c;
    }
}
