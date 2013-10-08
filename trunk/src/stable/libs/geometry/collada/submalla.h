#ifndef SUBMALLA_H
#define SUBMALLA_H

#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>

namespace Geometry {

    class SubMalla
    {

        /// \brief An enumeration of the geometric mesh primitives
        public: enum PrimitiveType {POINTS, LINES, LINESTRIPS, TRIANGLES,
                  TRIFANS, TRISTRIPS};


    public:
        SubMalla();

        public: virtual ~SubMalla();

        void Scale(double _factor);

        public: void setName(const std::string &_n);

        public: void setPrimitiveType(PrimitiveType _type);
        int getPrimitiveType() const;


        void addVertex(const Eigen::Vector3d &_v);
        void addIndex(unsigned int _i);
        unsigned int getVertexCount() const;

        void addNormal(const Eigen::Vector3d &_n);
        unsigned int getNormalCount() const;


        void addTexCoord(double _u, double _v);
        unsigned int getTexCoordCount() const;

        Eigen::Vector3d getVertex(unsigned int _i) const;
        Eigen::Vector3d getNormal(unsigned int _i) const;
        Eigen::Vector2d getTexCoord(unsigned int _i) const;

        unsigned int getIndex(unsigned int _i) const;
        unsigned int getIndexCount() const;

        public: void setMaterialIndex(unsigned int _index);
        unsigned int getMaterialIndex() const;



        void center();
        Eigen::Vector3d getMax() const;
        Eigen::Vector3d getMin() const;

        void translate(const Eigen::Vector3d &_vec);

        void FillArrays(float **_vertArr, int **_indArr) ;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        /// \brief the vertex array
        private: std::vector< Eigen::Vector3d > vertices;

        /// \brief the normal array
        private: std::vector< Eigen::Vector3d > normals;

        /// \brief the texture coordinate array
        private: std::vector< Eigen::Vector2d > texCoords;

        /// \brief the vertex index array
        private: std::vector<unsigned int> indices;

        /// mesh material list.
        private: int materialIndex;

        /// \brief The name of the sub-mesh
        private: std::string name;

        private: PrimitiveType primitiveType;
    };
}
#endif // SUBMALLA_H
