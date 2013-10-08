#include "matriz4x4.h"

namespace math
{

    Matriz4x4::Matriz4x4()
    {
        matriz  <<  1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
    }

    Matriz4x4::Matriz4x4(int type)
    {
        if(type==IDENTITY){
            matriz =Eigen::Matrix4f::Identity();
        }else if(type==ZEROS){
            matriz  << 0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0;
        }else{
            matriz =Eigen::Matrix4f::Identity();
        }
    }

    //////////////////////////////////////////////////
    void Matriz4x4::set(double _v00, double _v01, double _v02, double _v03,
                      double _v10, double _v11, double _v12, double _v13,
                      double _v20, double _v21, double _v22, double _v23,
                      double _v30, double _v31, double _v32, double _v33)
    {

        matriz << _v00, _v01, _v02, _v03,
                  _v10, _v11, _v12, _v13,
                  _v20, _v21, _v22, _v23,
                  _v30, _v31, _v32, _v33;
    }

    //////////////////////////////////////////////////
    void Matriz4x4::setScale( Eigen::Vector3d &_s)
    {
      matriz(0, 0) = _s(0);
      matriz(1, 1) = _s(1);
      matriz(2, 2) = _s(2);
      matriz(3, 3) = 1.0;
    }


    //////////////////////////////////////////////////
    void Matriz4x4::setTranslate( Eigen::Vector3d &_t)
    {
        matriz(0, 3) = _t(0);
        matriz(1, 3) = _t(1);
        matriz(2, 3) = _t(2);
    }

    Matriz4x4 Matriz4x4::operator*(Matriz3x3 _mat) const
    {

        Matriz4x4 r(math::Matriz4x4::IDENTITY);

        r.set( matriz(0, 0), matriz(0, 1), matriz(0, 2),matriz(0, 3),
               matriz(1, 0), matriz(1, 1), matriz(1, 2),matriz(1, 3),
               matriz(2, 0), matriz(2, 1), matriz(2, 2),matriz(2, 3),
               matriz(3, 0), matriz(3, 1), matriz(3, 2),matriz(3, 3));

        Eigen::Matrix3f m = r.matriz.block(0, 0, 3, 3);

        Eigen::Matrix3f resultado = m*_mat.getMatriz();

        r.set(resultado(0, 0), resultado(0, 1), resultado(0, 2),matriz(0, 3),
              resultado(1, 0), resultado(1, 1), resultado(1, 2),matriz(1, 3),
              resultado(2, 0), resultado(2, 1), resultado(2, 2),matriz(2, 3),
              matriz(3, 0), matriz(3, 1), matriz(3, 2),matriz(3, 3));
        return r;

    }

    Eigen::Matrix4f &Matriz4x4::getMatrix()
    {
        return this->matriz;
    }

    Eigen::Matrix4f Matriz4x4::getCopyMatrix()
    {
        return this->matriz;
    }

    void Matriz4x4::setMatrix(Eigen::Matrix4f m)
    {
        this->matriz = m;
    }


    Matriz4x4 Matriz4x4::operator*(Matriz4x4 &_mat) const
    {
        Eigen::Matrix4f m = _mat.getMatrix() * matriz;

        Matriz4x4 result;
        result.setMatrix(m);

        return result;
    }

    Eigen::Vector3d Matriz4x4::operator*(Eigen::Vector3d &_vec) const
    {
        Eigen::Vector3d result;
        result(0) = matriz(0, 0)*_vec(0) + matriz(0, 1)*_vec(1) +
                     matriz(0, 2)*_vec(2) + matriz(0, 3);

        result(1) = matriz(1, 0)*_vec(0) + matriz(1, 1)*_vec(1) +
                     matriz(1, 2)*_vec(2) + matriz(1, 3);

        result(2) = matriz(2, 0)*_vec(0) + matriz(2, 1)*_vec(1) +
                     matriz(2, 2)*_vec(2) + matriz(2, 3);
        return result;
    }
}
