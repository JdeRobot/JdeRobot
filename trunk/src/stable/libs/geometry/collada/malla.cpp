#include "malla.h"
namespace Geometry {

    Malla::Malla()
    {
    }

    //////////////////////////////////////////////////
    void Malla::setPath(const std::string &_path)
    {
      this->path = _path;
    }

    //////////////////////////////////////////////////
    void Malla::addSubMalla(SubMalla *_sub)
    {
      this->submeshes.push_back(_sub);
    }

    //////////////////////////////////////////////////
    void Malla::Scale(double _factor)
    {
      std::vector<SubMalla*>::iterator iter;
      for (iter = this->submeshes.begin(); iter != this->submeshes.end(); ++iter)
        (*iter)->Scale(_factor);
    }

    //////////////////////////////////////////////////
    int Malla::addMaterial(Material *_mat)
    {
      int result = -1;

      if (_mat)
      {
        this->materials.push_back(_mat);
        result = this->materials.size()-1;
      }

      return result;
    }

    //////////////////////////////////////////////////
    Material *Malla::getMaterial(int index)
    {
      if (index >= 0 && index < static_cast<int>(this->materials.size()))
        return this->materials[index];

      return NULL;
    }

    //////////////////////////////////////////////////
    unsigned int Malla::getSubMeshCount() const
    {
      return this->submeshes.size();
    }

    //////////////////////////////////////////////////
    SubMalla *Malla::getSubMesh(unsigned int i) const
    {
      if (i < this->submeshes.size())
        return this->submeshes[i];
      else
        std::cout << "Invalid index: " << i << " >= " << this->submeshes.size() << "\n";
    }

    //////////////////////////////////////////////////
    Eigen::Vector3d Malla::getMax() const
    {
      Eigen::Vector3d max;
      std::vector<SubMalla*>::const_iterator iter;

      max(0) = -std::numeric_limits<float>::max();
      max(1) = -std::numeric_limits<float>::max();
      max(2) = -std::numeric_limits<float>::max();

      for (iter = this->submeshes.begin(); iter != this->submeshes.end(); ++iter)
      {
        if ((*iter)->getVertexCount() <= 2)
          continue;

        Eigen::Vector3d smax = (*iter)->getMax();

        max(0) =std::max(max(0), smax(0));
        max(1) =std::max(max(1), smax(1));
        max(2) =std::max(max(2), smax(2));
      }

      return max;
    }

    //////////////////////////////////////////////////
    Eigen::Vector3d Malla::getMin() const
    {
      Eigen::Vector3d min;
      std::vector<SubMalla *>::const_iterator iter;

      min(0) = std::numeric_limits<float>::max();
      min(1) = std::numeric_limits<float>::max();
      min(2) = std::numeric_limits<float>::max();

      for (iter = this->submeshes.begin(); iter != this->submeshes.end(); ++iter)
      {
        if ((*iter)->getVertexCount() <= 2)
          continue;

        Eigen::Vector3d smin = (*iter)->getMin();
        min(0) = std::min(min(0), smin(0));
        min(1) = std::min(min(1), smin(1));
        min(2) = std::min(min(2), smin(2));
      }

      return min;
    }
}




