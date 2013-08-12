#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include <string>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
namespace files_3D {
    class Material
    {
    public:
        Material();

        public: void setDiffuse(const Color &_clr);
        Color getDiffuse() const;

        void setAmbient(const Color &_clr);
        Color getAmbient() const;

        void setEmissive(const Color &_clr);
        Color getEmissive() const;

        bool getLighting() const;
        double getShininess() const;

        std::string getTextureImage() const;


        void setTextureImage(const std::string &_tex);
        void setTextureImage(const std::string &_tex,
                                       const std::string &_resourcePath);
        void setTransparency(double _t);
        double getTransparency() const;
        void setBlendFactors(double _srcFactor, double _dstFactor);
        void setShininess(double _s);

        Color getSpecular() const;


        std::string getName() const;


        cv::Mat getImage();


    ///////////////////////////////////////////////////////////////////////////
        /// \brief the name of the material
         public: std::string name;

         /// \brief the texture image file name
         public: std::string texImage;

         /// \brief the ambient light color
         public: Color ambient;

         /// \brief the diffuse ligth color
         public: Color diffuse;

         /// \brief the specular light color
         public: Color specular;

         /// \brief the emissive light color
         public: Color emissive;

         /// \brief transparency value in the range 0 to 1
         public: double transparency;

         /// \brief shininess value (0 to 1)
         public: double shininess;

        /// \brief the total number of instanciated Material instances
        private: static unsigned int counter;

        /// \brief flag to perform depth buffer write
        private: bool depthWrite;

        private: bool lighting;

        /// \brief source blend factor
        private: double srcBlendFactor;

        /// \brief destination blend factor
        private: double dstBlendFactor;

        public: unsigned int idTextura ;

        cv::Mat image;
    };
}

#endif // MATERIAL_H
