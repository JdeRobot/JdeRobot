#include "malla.h"
namespace files_3D {
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
    math::Vector3 Malla::getMax() const
    {
      math::Vector3 max;
      std::vector<SubMalla*>::const_iterator iter;

      max.setX( -std::numeric_limits<float>::max());
      max.setY( -std::numeric_limits<float>::max());
      max.setZ( -std::numeric_limits<float>::max());

      for (iter = this->submeshes.begin(); iter != this->submeshes.end(); ++iter)
      {
        if ((*iter)->getVertexCount() <= 2)
          continue;

        math::Vector3 smax = (*iter)->getMax();

        max.setX(std::max(max.getX(), smax.getX()));
        max.setY(std::max(max.getY(), smax.getY()));
        max.setZ(std::max(max.getZ(), smax.getZ()));
      }

      return max;
    }

    //////////////////////////////////////////////////
    math::Vector3 Malla::getMin() const
    {
      math::Vector3 min;
      std::vector<SubMalla *>::const_iterator iter;

      min.setX( std::numeric_limits<float>::max());
      min.setY( std::numeric_limits<float>::max());
      min.setZ( std::numeric_limits<float>::max());

      for (iter = this->submeshes.begin(); iter != this->submeshes.end(); ++iter)
      {
        if ((*iter)->getVertexCount() <= 2)
          continue;

        math::Vector3 smin = (*iter)->getMin();
        min.setX(std::min(min.getX(), smin.getX()));
        min.setY(std::min(min.getY(), smin.getY()));
        min.setZ(std::min(min.getZ(), smin.getZ()));
      }

      return min;
    }
}




