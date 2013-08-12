#ifndef SUBMALLA_H
#define SUBMALLA_H

#include <vector>

#include "../math/vector3.h"
#include "../math/vector2d.h"
namespace files_3D {

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


        void addVertex(const math::Vector3 &_v);
        void addIndex(unsigned int _i);
        unsigned int getVertexCount() const;

        void addNormal(const math::Vector3 &_n);
        unsigned int getNormalCount() const;


        void addTexCoord(double _u, double _v);
        unsigned int getTexCoordCount() const;

        math::Vector3 getVertex(unsigned int _i) const;
        math::Vector3 getNormal(unsigned int _i) const;
        math::Vector2d getTexCoord(unsigned int _i) const;

        unsigned int getIndex(unsigned int _i) const;
        unsigned int getIndexCount() const;

        public: void setMaterialIndex(unsigned int _index);
        unsigned int getMaterialIndex() const;



        void center();
        math::Vector3 getMax() const;
        math::Vector3 getMin() const;

        void translate(const math::Vector3 &_vec);

        void FillArrays(float **_vertArr, int **_indArr) ;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        /// \brief the vertex array
        private: std::vector< math::Vector3 > vertices;

        /// \brief the normal array
        private: std::vector< math::Vector3 > normals;

        /// \brief the texture coordinate array
        private: std::vector< math::Vector2d > texCoords;

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
