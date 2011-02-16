#ifndef GSTPIPELINE_H
#define GSTPIPELINE_H

#include <exception>
#include <string>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/Cond.h>

namespace varcolorserver {

class Config
{
public:
  Config();
  bool validate() const;
  std::string toString() const;
  bool operator==( const Config & other );
  bool operator!=( const Config & other );
  
  //! video input uri file
  std::string uri;
  //! video output width [pixels]
  size_t width;
  //! video output height [pixels]
  size_t height;
  //! video output bits per pixel
  size_t bpp;
  //! video output frame rate numerator
  size_t framerateN;
  //! video output frame rate denominator
  size_t framerateD;
  //! video output format string
  std::string format;
};

class GSTPipelineConfigException: virtual std::exception {};
  

extern int gst_init(int* argcp, char** argvp[]);
extern GError* err;

class GSTPipeline{
public:
  GSTPipeline(const Config &cfg) throw(GSTPipelineConfigException);
  ~GSTPipeline();

  virtual GstStateChangeReturn play();
  virtual GstStateChangeReturn pause();

  GstBuffer* pull_buffer();
private:
  static gboolean my_bus_callback(GstBus *bus,
				  GstMessage *message,
				  gpointer data);
  static void cb_newpad (GstElement *decode,
			 GstPad *pad,
			 gboolean last,
			 gpointer data);
  GstElement *pipeline,*source,*decoder,*videoscale,*videorate,*videocolor,*filter,*sink,*bin;
  GstBus *bus;
  IceUtil::ThreadPtr mainLoopTh;
  Config config_;
};

}

#endif /*GSTPIPELINE_H*/
