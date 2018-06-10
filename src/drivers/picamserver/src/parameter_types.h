#ifndef _PICAM_TYPES_H
#define _PICAM_TYPES_H

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/assert.hpp>
#include <map>


using namespace std;
using namespace boost::assign; // bring 'map_list_of()' into scope


namespace piCam {

    /**Image formats
     */
    enum PICAM_FORMAT{
        PICAM_FORMAT_YUV420,
        PICAM_FORMAT_GRAY,
        PICAM_FORMAT_BGR,
        PICAM_FORMAT_RGB,

    };

    extern map<string, PICAM_FORMAT> format_map;

    /**Exposure types
     */
    enum PICAM_EXPOSURE {
        PICAM_EXPOSURE_OFF,
        PICAM_EXPOSURE_AUTO,
        PICAM_EXPOSURE_NIGHT,
        PICAM_EXPOSURE_NIGHTPREVIEW,
        PICAM_EXPOSURE_BACKLIGHT,
        PICAM_EXPOSURE_SPOTLIGHT,
        PICAM_EXPOSURE_SPORTS,
        PICAM_EXPOSURE_SNOW,
        PICAM_EXPOSURE_BEACH,
        PICAM_EXPOSURE_VERYLONG,
        PICAM_EXPOSURE_FIXEDFPS,
        PICAM_EXPOSURE_ANTISHAKE,
        PICAM_EXPOSURE_FIREWORKS
    }  ;

      extern map<string, PICAM_EXPOSURE> exposure_map;




    /**Auto white balance types
     */
    enum PICAM_AWB {
        PICAM_AWB_OFF,
        PICAM_AWB_AUTO,
        PICAM_AWB_SUNLIGHT,
        PICAM_AWB_CLOUDY,
        PICAM_AWB_SHADE,
        PICAM_AWB_TUNGSTEN,
        PICAM_AWB_FLUORESCENT,
        PICAM_AWB_INCANDESCENT,
        PICAM_AWB_FLASH,
        PICAM_AWB_HORIZON
    }  ;


    extern map<string, PICAM_AWB> awb_map;


    /**Image effects
     */
    enum PICAM_IMAGE_EFFECT {
        PICAM_IMAGE_EFFECT_NONE,
        PICAM_IMAGE_EFFECT_NEGATIVE,
        PICAM_IMAGE_EFFECT_SOLARIZE,
        PICAM_IMAGE_EFFECT_SKETCH,
        PICAM_IMAGE_EFFECT_DENOISE,
        PICAM_IMAGE_EFFECT_EMBOSS,
        PICAM_IMAGE_EFFECT_OILPAINT,
        PICAM_IMAGE_EFFECT_HATCH,
        PICAM_IMAGE_EFFECT_GPEN,
        PICAM_IMAGE_EFFECT_PASTEL,
        PICAM_IMAGE_EFFECT_WATERCOLOR,
        PICAM_IMAGE_EFFECT_FILM,
        PICAM_IMAGE_EFFECT_BLUR,
        PICAM_IMAGE_EFFECT_SATURATION,
        PICAM_IMAGE_EFFECT_COLORSWAP,
        PICAM_IMAGE_EFFECT_WASHEDOUT,
        PICAM_IMAGE_EFFECT_POSTERISE,
        PICAM_IMAGE_EFFECT_COLORPOINT,
        PICAM_IMAGE_EFFECT_COLORBALANCE,
        PICAM_IMAGE_EFFECT_CARTOON
    } ;

    extern map<string, PICAM_IMAGE_EFFECT> effect_map;



    /**Metering types
     */
    enum PICAM_METERING {
        PICAM_METERING_AVERAGE,
        PICAM_METERING_SPOT,
        PICAM_METERING_BACKLIT,
        PICAM_METERING_MATRIX
    }  ;

    extern map<string, PICAM_METERING> metering_map;



    /*Encoding modes (for still mode)
     */

    typedef enum PICAM_ENCODING {
        PICAM_ENCODING_JPEG,
        PICAM_ENCODING_BMP,
        PICAM_ENCODING_GIF,
        PICAM_ENCODING_PNG,
        PICAM_ENCODING_RGB
    } PICAM_ENCODING;


}
#endif
