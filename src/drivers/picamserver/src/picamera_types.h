#ifndef _piCam_core_types_H
#define _piCam_core_types_H

namespace piCam {


        /// struct contain camera settings
        struct MMAL_PARAM_COLOURFX_T
        {
            int enable,u,v;       /// Turn colourFX on or off, U and V to use
        } ;
        struct PARAM_FLOAT_RECT_T
        {
            double x,y,w,h;
        } ;


        /** Structure containing all state information for the current run
         */
        struct RASPIVID_STATE
        {
            int sensor_mode;                          /// Requested width of image
            int width;                          /// Requested width of image
            int height;                         /// requested height of image
            int framerate;                      /// Requested frame rate (fps)
            /// the camera output or the encoder output (with compression artifacts)
            MMAL_COMPONENT_T *camera_component;    /// Pointer to the camera component
            MMAL_POOL_T *video_pool; /// Pointer to the pool of buffers used by encoder output port
            //camera params
            int sharpness;             /// -100 to 100
            int contrast;              /// -100 to 100
            int brightness;            ///  0 to 100
            int saturation;            ///  -100 to 100
            int ISO;
            bool videoStabilisation;    /// 0 or 1 (false or true)
            int exposureCompensation;  /// -10 to +10 ?
            int shutterSpeed;
	    PICAM_FORMAT captureFtm;
            PICAM_EXPOSURE rpc_exposureMode;
            PICAM_METERING rpc_exposureMeterMode;
            PICAM_AWB rpc_awbMode;
            PICAM_IMAGE_EFFECT rpc_imageEffect;
            MMAL_PARAMETER_IMAGEFX_PARAMETERS_T imageEffectsParameters;
            MMAL_PARAM_COLOURFX_T colourEffects;
            int rotation;              /// 0-359
            int hflip;                 /// 0 or 1
            int vflip;                 /// 0 or 1
            PARAM_FLOAT_RECT_T  roi;   /// region of interest to use on the sensor. Normalised [0,1] values in the rect
            float awbg_red;//white balance red and blue
            float awbg_blue;
        } ;

        //clean buffer
        template<typename T>
        class membuf{
            public:
            membuf() {
                data=0;
                size=0;
            }
            ~membuf() {
                if ( data!=0 ) delete []data;
            }
            void resize ( size_t s ) {
                if ( s!=size ) {
                    delete data;
                    size=s;
                    data=new  T[size];
                }
            }
            T *data;
            size_t size;
        };

}

#endif
