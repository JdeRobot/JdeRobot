#include "submalla.h"
namespace Geometry {

    SubMalla::SubMalla()
    {
        materialIndex = -1;
    }

    SubMalla::~SubMalla()
    {
    }

    //////////////////////////////////////////////////
    void SubMalla::setName(const std::string &_n)
    {
      this->name = _n;
    }

    //////////////////////////////////////////////////
    void SubMalla::Scale(double _factor)
    {
      for (std::vector<Eigen::Vector3d>::iterator iter = this->vertices.begin();
           iter != this->vertices.end(); ++iter)
      {
          (*iter)(0) *= _factor;
          (*iter)(1) *= _factor;
          (*iter)(2) *= _factor;
      }
    }

    //////////////////////////////////////////////////
    void SubMalla::setPrimitiveType(PrimitiveType _type)
    {
      this->primitiveType = _type;
    }

    //////////////////////////////////////////////////
    int SubMalla::getPrimitiveType() const
    {
      return this->primitiveType;
    }

    //////////////////////////////////////////////////
    void SubMalla::addVertex(const Eigen::Vector3d &_v)
    {
      this->vertices.push_back(_v);
    }

    //////////////////////////////////////////////////
    void SubMalla::addIndex(unsigned int _i)
    {
      this->indices.push_back(_i);
    }

    //////////////////////////////////////////////////
    unsigned int SubMalla::getVertexCount() const
    {
      return this->vertices.size();
    }

    //////////////////////////////////////////////////
    Eigen::Vector3d SubMalla::getVertex(unsigned int _i) const
    {
      if (_i >= this->vertices.size())
        std::cout << "Index too large\n";

      return this->vertices[_i];
    }

    //////////////////////////////////////////////////
    Eigen::Vector3d SubMalla::getNormal(unsigned int _i) const
    {
      if (_i >= this->normals.size())
        std::cout <<"Index too large";

      return this->normals[_i];
    }

    //////////////////////////////////////////////////
    unsigned int SubMalla::getNormalCount() const
    {
      return this->normals.size();
    }

    //////////////////////////////////////////////////
    void SubMalla::setMaterialIndex(unsigned int _index)
    {
      this->materialIndex = _index;
    }

    //////////////////////////////////////////////////
    unsigned int SubMalla::getMaterialIndex() const
    {
      return this->materialIndex;
    }

    //////////////////////////////////////////////////
    void SubMalla::addNormal(const Eigen::Vector3d &_n)
    {
      this->normals.push_back(_n);
    }

    //////////////////////////////////////////////////
    void SubMalla::addTexCoord(double _u, double _v)
    {
      this->texCoords.push_back(Eigen::Vector2d(_u, _v));
    }

    //////////////////////////////////////////////////
    unsigned int SubMalla::getTexCoordCount() const
    {
      return this->texCoords.size();
    }

    //////////////////////////////////////////////////
    unsigned int SubMalla::getIndexCount() const
    {
      return this->indices.size();
    }


    //////////////////////////////////////////////////
    unsigned int SubMalla::getIndex(unsigned int _i) const
    {
      if (_i > this->indices.size())
        std::cout << "Index too large";

      return this->indices[_i];
    }

    //////////////////////////////////////////////////
    Eigen::Vector2d SubMalla::getTexCoord(unsigned int _i) const
    {
      if (_i >= this->texCoords.size())
        std::cout << "Index too large";

      return this->texCoords[_i];
    }

    //////////////////////////////////////////////////
    void SubMalla::center()
    {
     Eigen::Vector3d _center(0, 0, 0);

      Eigen::Vector3d min, max, half;
      min = this->getMin();
      max = this->getMax();
      half = (max - min) * 0.5;

      this->translate(_center - (min + half));
    }

    //////////////////////////////////////////////////
    void SubMalla::translate(const Eigen::Vector3d &_vec)
    {
      for (std::vector<Eigen::Vector3d>::iterator iter = this->vertices.begin();
           iter != this->vertices.end(); ++iter)
      {
        (*iter) += _vec;
      }
    }

    //////////////////////////////////////////////////
    Eigen::Vector3d SubMalla::getMax() const
    {
      Eigen::Vector3d max;
      std::vector<Eigen::Vector3d>::const_iterator iter;

      max(0) = -std::numeric_limits<float>::max();
      max(1) = -std::numeric_limits<float>::max();
      max(2) = -std::numeric_limits<float>::max();

      for (iter = this->vertices.begin(); iter != this->vertices.end(); ++iter)
      {
        Eigen::Vector3d v = (*iter);
        max(0) = std::max(max(0), v(0));
        max(1) = std::max(max(1), v(1));
        max(2) = std::max(max(2), v(2));
      }

      return max;
    }

    //////////////////////////////////////////////////
    Eigen::Vector3d SubMalla::getMin() const
    {
      Eigen::Vector3d min;
      std::vector<Eigen::Vector3d>::const_iterator iter;

      min(0) = std::numeric_limits<float>::max();
      min(1) = std::numeric_limits<float>::max();
      min(2) = std::numeric_limits<float>::max();

      for (iter = this->vertices.begin(); iter != this->vertices.end(); ++iter)
      {
        Eigen::Vector3d v = (*iter);
        min(0) = std::min(min(0), v(0));
        min(1) = std::min(min(1), v(1));
        min(2) = std::min(min(2), v(2));

      }

      return min;
    }

    //////////////////////////////////////////////////
    // PAra ODE
    ////
    void SubMalla::FillArrays(float **_vertArr, int **_indArr)
    {
      if (this->vertices.size() == 0 || this->indices.size() == 0)
        std::cout << "No vertices or indices\n";

      std::vector< Eigen::Vector3d >::const_iterator viter;
      std::vector< unsigned int >::const_iterator iiter;
      unsigned int i;

      if (*_vertArr)
        delete [] *_vertArr;

      if (*_indArr)
        delete [] *_indArr;

      *_vertArr = new float[this->vertices.size() * 3];
      *_indArr = new int[this->indices.size()];

      for (viter = this->vertices.begin(), i = 0; viter != this->vertices.end();
          ++viter){
          (*_vertArr)[i++] = static_cast<float>((*viter)(0));
          (*_vertArr)[i++] = static_cast<float>((*viter)(1));
          (*_vertArr)[i++] = static_cast<float>((*viter)(2));
      }

      for (iiter = this->indices.begin(), i = 0;
          iiter != this->indices.end(); ++iiter)
        (*_indArr)[i++] = (*iiter);
    }
}




