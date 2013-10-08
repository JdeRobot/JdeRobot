#ifndef MATRIZ4X4_H
#define MATRIZ4X4_H

#define EIGEN_DONT_ALIGN_STATICALLY True

#include <eigen3/Eigen/Dense>
#include "matriz3x3.h"

namespace math
{


    class Matriz4x4
    {
    public: enum typeMatriz { IDENTITY, ZEROS };

    public:
        Matriz4x4();
        Matriz4x4(int type);
        Eigen::Matrix4f &getMatrix();
        Eigen::Matrix4f getCopyMatrix();
        void setMatrix(Eigen::Matrix4f m);

        void setTranslate(Eigen::Vector3d &_t);
        void setScale(Eigen::Vector3d &_s);

        void set(double _v00, double _v01, double _v02, double _v03,
                 double _v10, double _v11, double _v12, double _v13,
                 double _v20, double _v21, double _v22, double _v23,
                 double _v30, double _v31, double _v32, double _v33);

        public: Matriz4x4 operator*(Matriz3x3 _mat) const;
        public: Matriz4x4 operator*( Matriz4x4 &_mat) const;
        public: Eigen::Vector3d operator*(Eigen::Vector3d &_vec) const;
    //private:
        Eigen::Matrix4f matriz;
    };

}

#endif // MATRIZ4X4_H
