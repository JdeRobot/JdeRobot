#ifndef _piCam_core_H
#define _piCam_core_H
#include "interface/mmal/mmal.h"
#include "interface/mmal/util/mmal_connection.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <mutex>
#include <iostream>
#include <string>
#include "parameter_types.h"
#include "picamera_types.h"
#include "threadcondition.h"
#include "easyiceconfig/EasyIce.h"

namespace piCam {

        class PiCamCore
        {
            struct PORT_USERDATA
            {
                PORT_USERDATA() {
                    wantToGrab=false;
                    pstate=0;
                }
                void waitForFrame() {
                    //_mutex.lock();
                    std::unique_lock<std::mutex> lck ( _mutex );

                    wantToGrab=true;
//                    _mutex.unlock();
//                    Thcond.Wait();
                       Thcond.Wait(lck); //this will unlock the mutex and wait atomically
                };



                RASPIVID_STATE *pstate;            /// pointer to our state in case required in callback
                PiCamCore *inst;
                std::mutex _mutex;
                ThreadCondition Thcond;
                bool wantToGrab;
                membuf<unsigned char> _buffData;

                /* User define callback interface */
                void (*_userCallback)(void*) = 0;
                void* _userCallbackData;

            };

            public:

            /**Constructor
             */
            PiCamCore();
            /**Destructor
             */
            PiCamCore(const piCam::PiCamCore &obj);

            ~PiCamCore();
            /**Opens the camera and start capturing
            */
            bool open ( bool StartCapture=true );
            /**indicates if camera is open
            */
            bool isOpened() const
            {
                return _isOpened;
            }
            /**Starts camera capture
             */
            bool startCapture();
            /**Indicates if is capturing
             */
            bool isCapturing() const{return _isCapturing;}

            /*
             * the function 'userCallback' will be called every time new data arrived from camera,
             * with 'data' as argument.
             */
            void setUserCallback(void (*userCallback)(void*) , void* data = 0);

            /**Grabs the next frame and keeps it in internal buffer. Blocks until next frame arrives
            */
            bool grab();
            /**Retrieves the buffer previously grabbed.
             */

            void retrieve ( cv::Mat& image,PICAM_FORMAT type );

            unsigned char *getImageBufferData() const;
            /**
             * Returns the size of the buffer returned in getImagePtr. If is like calling getImageTypeSize(getFormat()). Just for dummies :P
             */
            size_t getImageBufferSize() const;

            /** Stops camera and free resources
            */
            void release();

            //sets capture format. Can not be changed once camera is opened
            void setFormat ( PICAM_FORMAT fmt );
            //sets sensor mode. Can not be changed once camera is opened
            void setSensorMode ( int mode );

            void setWidth ( unsigned int width ) ;
            void setHeight ( unsigned int height );
            void setCaptureSize ( unsigned int width, unsigned int height );
            void setBrightness ( unsigned int brightness );
            void setRotation ( int rotation );
            void setISO ( int iso );
            void setSharpness ( int sharpness );
            void setContrast ( int contrast );
            void setSaturation ( int saturation );
            void setExposure ( PICAM_EXPOSURE exposure );
            void setVideoStabilization ( bool v );
            void setExposureCompensation ( int val ); //-10,10
            void setAWB ( PICAM_AWB awb );
            void setAWB_RB ( float red,float blue );//ranges [0,1]

            void setImageEffect ( PICAM_IMAGE_EFFECT imageEffect );
            void setMetering ( PICAM_METERING metering );
            void setHorizontalFlip ( bool hFlip );
            void setVerticalFlip ( bool vFlip );

            void setShutterSpeed ( unsigned int shutter ); //currently not  supported
            void setFrameRate ( int fps );

            PICAM_FORMAT  getFormat() const {return State.captureFtm;}
            //Accessors
            unsigned int getSensorMode() const
            {
                return State.sensor_mode;
            }
            unsigned int getWidth() const
            {
                return State.width;
            }
            unsigned int getHeight() const
            {
                return State.height;
            }
            unsigned int getBrightness() const
            {
                return State.brightness;
            }
            unsigned int getRotation() const
            {
                return State.rotation;
            }
            int getISO() const
            {
                return State.ISO;
            }
            int getSharpness() const
            {
                return State.sharpness;
            }
            int getContrast() const
            {
                return State.contrast;
            }
            int getSaturation() const
            {
                return State.saturation;
            }
            unsigned int getShutterSpeed() const
            {
                return State.shutterSpeed;
            }
            PICAM_EXPOSURE getExposure() const
            {
                return State.rpc_exposureMode;
            }
            PICAM_AWB getAWB() const
            {
                return State.rpc_awbMode;
            }

            float getAWBG_red(){return State.awbg_red;}

            float getAWBG_blue(){return State.awbg_blue;}

            int getFrameRate() const
            {
                return State.framerate;
            }

            PICAM_IMAGE_EFFECT getImageEffect() const
            {
                return State.rpc_imageEffect;
            }
            PICAM_METERING getMetering() const
            {
                return State.rpc_exposureMeterMode;
            }
            bool isHorizontallyFlipped() const
            {
                return State.hflip;
            }
            bool isVerticallyFlipped() const
            {
                return State.vflip;
            }



            std::string getId() const;

            size_t getImageTypeSize ( PICAM_FORMAT type ) const;


            static void video_buffer_callback ( MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer );
            static void camera_control_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
            void setDefaultStateParams();
            void setCustomStateParams(  Ice::PropertiesPtr prop );
            MMAL_COMPONENT_T *create_camera_component ( RASPIVID_STATE *state );
            void destroy_camera_component ( RASPIVID_STATE *state );


            //Commit
            void commitParameters( );
            void commitBrightness();
            void commitRotation() ;
            void commitISO() ;
            void commitSharpness();
            void commitContrast();
            void commitSaturation();
            void commitExposure();
            void commitAWB();
            void commitImageEffect();
            void commitMetering();
            void commitFlips();
            void commitExposureCompensation();
            void commitVideoStabilization();
            void commitShutterSpeed();
            void commitAWB_RB();

            MMAL_PARAM_EXPOSUREMODE_T convertExposure ( PICAM_EXPOSURE exposure ) ;
            MMAL_PARAM_AWBMODE_T  convertAWB ( PICAM_AWB awb ) ;
            MMAL_PARAM_IMAGEFX_T convertImageEffect ( PICAM_IMAGE_EFFECT imageEffect ) ;
            MMAL_PARAM_EXPOSUREMETERINGMODE_T convertMetering ( PICAM_METERING metering ) ;
            int convertFormat ( PICAM_FORMAT fmt ) ;


            //Color conversion
	    void convertBGR2RGB(unsigned char *  in_bgr,unsigned char *  out_rgb,int size);
            float VIDEO_FRAME_RATE_NUM;
            RASPIVID_STATE State;
            MMAL_STATUS_T status;
            MMAL_PORT_T *camera_video_port;//,*camera_still_port
            PORT_USERDATA callback_data;
            bool _isOpened;
            bool _isCapturing;

            bool _rgb_bgr_fixed;


        };

}

#endif
