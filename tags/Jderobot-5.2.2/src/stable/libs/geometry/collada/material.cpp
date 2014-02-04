#include "material.h"

namespace Geometry {

    unsigned int Material::counter = 0;

    Material::Material()
    {
      this->name = "gazebo_material_" + boost::lexical_cast<std::string>(counter++);
      this->specular.set(0, 0, 0, 1);
      this->lighting = false;
    }

    //////////////////////////////////////////////////
    void Material::setDiffuse(const Color &_clr)
    {
      this->diffuse = _clr;
      this->lighting = true;
    }
    //////////////////////////////////////////////////
    Color Material::getDiffuse() const
    {
      return this->diffuse;
    }

    //////////////////////////////////////////////////
    void Material::setAmbient(const Color &_clr)
    {
      this->ambient = _clr;
    }

    //////////////////////////////////////////////////
    Color Material::getAmbient() const
    {
      return this->ambient;
    }

    //////////////////////////////////////////////////
    void Material::setEmissive(const Color &_clr)
    {
      this->emissive = _clr;
    }

    //////////////////////////////////////////////////
    Color Material::getEmissive() const
    {
      return this->emissive;
    }

    //////////////////////////////////////////////////
    double Material::getShininess() const
    {
      return this->shininess;
    }


    //////////////////////////////////////////////////
    bool Material::getLighting() const
    {
      return this->lighting;
    }

    //////////////////////////////////////////////////
    void Material::setTextureImage(const std::string &_tex)
    {
      this->texImage = _tex;
    }

    //////////////////////////////////////////////////
    void Material::setTextureImage(const std::string &_tex,
                                   const std::string &_resourcePath)
    {
      this->texImage = _resourcePath + "/" + _tex;

      // If the texture image doesn't exist then try the next most likely path.
      if (!boost::filesystem::exists(this->texImage))
      {
        this->texImage = _resourcePath + "./" + _tex;
        if (!boost::filesystem::exists(this->texImage))
        {
          std::cout << "Unable to find texture[" << _tex << "] in path["
                << _resourcePath << "]\n";
        }else{
            std::cout << "find texture[" << _tex << "] in path["
                  << _resourcePath << "]\n";
            image = cv::imread(this->texImage);
        }
      }
    }

    cv::Mat Material::getImage()
    {
        return image;
    }

    //////////////////////////////////////////////////
    void Material::setTransparency(double _t)
    {
      this->transparency = std::min(_t, 1.0);
      this->transparency = std::max(this->transparency, 0.0);
      this->lighting = true;
    }

    //////////////////////////////////////////////////
    Color Material::getSpecular() const
    {
      return this->specular;
    }

    //////////////////////////////////////////////////
    double Material::getTransparency() const
    {
      return this->transparency;
    }

    //////////////////////////////////////////////////
    void Material::setBlendFactors(double _srcFactor, double _dstFactor)
    {
      this->srcBlendFactor = _srcFactor;
      this->dstBlendFactor = _dstFactor;
    }

    //////////////////////////////////////////////////
    void Material::setShininess(double _s)
    {
      this->shininess = _s;
      this->lighting = true;
    }

    //////////////////////////////////////////////////
    std::string Material::getTextureImage() const
    {
      return this->texImage;
    }

    //////////////////////////////////////////////////
    std::string Material::getName() const
    {
      return this->name;
    }
}

