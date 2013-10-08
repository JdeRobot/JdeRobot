#ifndef COLLADAPARSER_H
#define COLLADAPARSER_H

#include <iostream>
#include <vector>
#include <map>

#include <tinyxml.h>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include "malla.h"
#include "submalla.h"
#include "color.h"

#include "../math/utils.h"
#include "../math/matriz4x4.h"
#include "../math/matriz3x3.h"
#include "../math/plano.h"
#include "../math/segmento.h"
#include "../math/Point3D.h"
#include <eigen3/Eigen/Dense>

#include <GL/glut.h>
#include <GL/gl.h>

namespace Geometry {

    class ColladaParser
    {
    public:
        ColladaParser(std::string filename, bool to2D=false,  double scalemap=1, double scale=1);

        void worldTo2D();
        cv::Mat getWorld2D();


        void loadScene(Malla *_mesh);

        void draw();

        TiXmlElement *getElementId(const std::string &_name,
                                   const std::string &_id);

        TiXmlElement *getElementId(TiXmlElement *_parent,
                                    const std::string &_name,
                                    const std::string &_id);

        float loadFloat(TiXmlElement *_elem);
        void loadTransparent(TiXmlElement *_elem, Material *_mat);


        void loadNode(TiXmlElement *_elem,
                      Malla *_mesh,
                      const math::Matriz4x4 &_transform);

        void loadColorOrTexture(TiXmlElement *_elem,
            const std::string &_type, Material *_mat);

        math::Matriz4x4 loadNodeTransform(TiXmlElement *_elem);

        void loadGeometry(TiXmlElement *_xml,
                          const math::Matriz4x4 &_transform, Malla *_mesh);

        void loadVertices(const std::string &_id,
                        const math::Matriz4x4 &_transform,
                        std::vector<Eigen::Vector3d> &_verts,
                        std::vector<Eigen::Vector3d> &_norms);

        void loadNormals(const std::string &_id,
                                        const math::Matriz4x4 &_transform,
                                        std::vector<Eigen::Vector3d> &_values);

        void loadPositions(const std::string &_id,
                            const math::Matriz4x4 &_transform,
                            std::vector<Eigen::Vector3d> &_values);

        void loadController(TiXmlElement *_contrXml,
              TiXmlElement *_skelXml, const math::Matriz4x4 _transform, Malla *_mesh);

        void loadPolylist(TiXmlElement *_polylistXml,
                        const math::Matriz4x4 &_transform,
                        Malla *_mesh);

        void loadLines(TiXmlElement *_xml,
                        const math::Matriz4x4 &_transform,
                        Malla *_mesh);

        void loadTriangles(TiXmlElement *_trianglesXml,
                                          const math::Matriz4x4 &_transform,
                                          Malla *_mesh);
        Material *loadMaterial(const std::string &_name);
        void loadTexCoords(const std::string &_id,
                                          std::vector<Eigen::Vector2d> &_values);
        // collada filename
        private: std::string filename;
        private: std::string path;

        // root xml element
        private: TiXmlElement *colladaXml;

        // scale factor
        private: double meter;

        // Name of the current node.
        private: std::string currentNodeName;

        // material dictionary indexed by name
        private: std::map<std::string, std::string> materialMap;

    public: Malla *mesh;

    public: cv::Mat image;

    private:

      int ite;


    };
}

#endif // COLLADAPARSER_H
