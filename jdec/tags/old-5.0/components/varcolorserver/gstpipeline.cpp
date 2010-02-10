#include "gstpipeline.h"
#include <formats.h>
#include <iostream>

namespace gstvideopipeline {

Config::Config()
  :   uri(""),
      width(0),
      height(0),
      bpp(0),
      framerateN(0),
      framerateD(0),
      format("")
{
}

bool
Config::validate() const
{
  if ( uri == "" ) return false;
  if ( width <= 0 ) return false;
  if ( height <= 0 ) return false;
  if ( bpp <= 0 ) return false;
  if ( framerateN <= 0 ) return false;
  if ( framerateD <= 0 ) return false;
  if ( format == "" ) return false;
  return true;
}

std::string
Config::toString() const
{
    std::stringstream ss;
    ss << "GSTVideoPipeline config: uri=" << uri << 
      " width=" << width << " height=" << height << 
      " bpp=" << bpp << " framerate=" << framerateN << "/" << framerateD << 
      " format=" << format;
    return ss.str();
}

bool 
Config::operator==( const Config & other )
{
    return (uri==other.uri && width==other.width && 
	    height==other.height && bpp==other.bpp && 
	    framerateN==other.framerateN && framerateD==other.framerateD && 
	    format==other.format);
}

bool 
Config::operator!=( const Config & other )
{
  return !(*this==other);
}

int gst_init(int* argcp, char** argvp[]){
  if (g_thread_supported())
    g_thread_init(NULL);
  return gst_init_check(argcp,argvp,&err);
}

/*FIXME: throw exception*/
GError* err;


class GSTMainLoopThread: public IceUtil::Thread {
public: 
  GSTMainLoopThread(){
    mainloop = g_main_loop_new (NULL, FALSE);
  }

  virtual ~GSTMainLoopThread(){
    if (g_main_loop_is_running(mainloop))
      g_main_loop_quit(mainloop);
    
    getThreadControl().join();
    g_main_loop_unref (mainloop);
  }

  virtual void run(){
    g_main_loop_run (mainloop);
  }

private:
  GMainLoop *mainloop;
};

GSTPipeline::GSTPipeline(const Config &cfg)
  :config_(cfg)
{
  GstCaps *caps;
  const Format *pixelFormat;
  
  /* create pipeline, add handler */
  pipeline = gst_pipeline_new ("my_pipeline");
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  gst_bus_add_watch (bus, GSTPipeline::my_bus_callback, (void*)this);
  gst_object_unref (bus);

  pixelFormat = searchPixelFormat(config_.format.c_str());

  if (config_.format.find("RGB") == 0){
    caps = gst_caps_new_simple ("video/x-raw-rgb",
				"bpp", G_TYPE_INT,pixelFormat->bitsPerPixel,
				"depth",G_TYPE_INT,pixelFormat->bitsPerPixel,
				"red_mask",G_TYPE_INT,pixelFormat->componetsMask[INDEX_RED],
				"green_mask",G_TYPE_INT,pixelFormat->componetsMask[INDEX_GREEN],
				"blue_mask",G_TYPE_INT,pixelFormat->componetsMask[INDEX_BLUE],
				"alpha_mask",G_TYPE_INT,pixelFormat->componetsMask[INDEX_ALPHA],
				"width", G_TYPE_INT, config_.width,
				"height", G_TYPE_INT, config_.height,
				"framerate", GST_TYPE_FRACTION, 
				config_.framerateN, config_.framerateD,
				NULL);
  }else{
    caps = gst_caps_new_simple ("video/x-raw-yuv",
				"width", G_TYPE_INT, config_.width,
				"height", G_TYPE_INT, config_.height,
				"framerate", GST_TYPE_FRACTION, 
				config_.framerateN, config_.framerateD,
				NULL);
  }

  if (config_.uri.find("v4l://") == 0){/*handle v4l source*/
    std::string dev = config_.uri.substr(6);/*after v4l://*/
    std::cerr << dev << std::endl;
    source = gst_element_factory_make("v4l2src","source");
    g_object_set(G_OBJECT(source),"device",dev.c_str(),NULL);
    sink = gst_element_factory_make("appsink","sink");
    g_object_set(G_OBJECT(sink),"drop",1,NULL);
    g_object_set(G_OBJECT(sink),"max-buffers",16,NULL);

    videocolor = gst_element_factory_make("ffmpegcolorspace","videocolor");
    gst_bin_add_many(GST_BIN(pipeline),source,videocolor,sink,NULL);
    gst_element_link(source,videocolor);
    gst_element_link_filtered(videocolor,sink,caps);
  }else if(config_.uri.find("videotest://") == 0){/*handle videotest source*/
    std::string pattern = config_.uri.substr(11);/*after videotest://*/
    source = gst_element_factory_make("videotest","source");
    g_object_set(G_OBJECT(source),"pattern",pattern.c_str(),NULL);
    sink = gst_element_factory_make("appsink","sink");

    g_object_set(G_OBJECT(sink),"drop",1,NULL);
    g_object_set(G_OBJECT(sink),"max-buffers",16,NULL);
    gst_bin_add_many(GST_BIN(pipeline),source,sink,NULL);
    gst_element_link_filtered(source,sink,caps);
  }else{
    source = gst_element_make_from_uri(GST_URI_SRC,config_.uri.c_str(),
				       "source");
    decoder = gst_element_factory_make("decodebin","decoder");
    g_signal_connect (decoder, "new-decoded-pad", G_CALLBACK (cb_newpad), (void*)this);
  
    videoscale = gst_element_factory_make("videoscale","videoscale");
    videorate = gst_element_factory_make("videorate","videorate");
    videocolor = gst_element_factory_make("ffmpegcolorspace","videocolor");
    sink = gst_element_factory_make("appsink","sink");

    g_object_set(G_OBJECT(sink),"drop",1,NULL);
    g_object_set(G_OBJECT(sink),"max-buffers",16,NULL);
    gst_bin_add_many(GST_BIN(pipeline),source,videorate,videoscale,videocolor,sink,NULL);
  
    gst_element_link(source,decoder);
    gst_element_link(videorate,videoscale);
    gst_element_link(videoscale,videocolor);
    gst_element_link_filtered(videocolor,sink,caps);
  }
  
  gst_caps_unref (caps);
  gst_element_set_state (GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  
  mainLoopTh = new GSTMainLoopThread();
  mainLoopTh->start();
}
  
GSTPipeline::~GSTPipeline(){
  gst_element_set_state (GST_ELEMENT(pipeline), GST_STATE_NULL);
  gst_object_unref (pipeline);
  
}

GstStateChangeReturn GSTPipeline::play(){
  return gst_element_set_state (GST_ELEMENT(pipeline), GST_STATE_PLAYING);
}

GstStateChangeReturn GSTPipeline::pause(){
  return gst_element_set_state (pipeline, GST_STATE_PAUSED);
}
  
GstBuffer* GSTPipeline::pull_buffer(){
  /*FIXME: how to restart??*/
  if (gst_app_sink_is_eos(GST_APP_SINK(sink)))
    return NULL;
  else
    return gst_app_sink_pull_buffer(GST_APP_SINK(sink));
}

void
GSTPipeline::cb_newpad (GstElement *decode,
			GstPad     *pad,
			gboolean    last,
			gpointer    data)
{
  GstCaps *caps;
  GstStructure *str;
  GstPad *videopad;
  GSTPipeline *self = static_cast<GSTPipeline*>(data);

  /* only link once */
  videopad = gst_element_get_static_pad (self->videorate, "sink");
  if (GST_PAD_IS_LINKED (videopad)) {
    gst_object_unref (videopad);
    return;
  }

  /* check media type */
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "video")) {
    gst_caps_unref (caps);
    gst_object_unref (pad);
    return;
  }
  gst_caps_unref (caps);

  /* link'n'play */
  gst_pad_link (pad, videopad);
  gst_object_unref (videopad);
}

gboolean GSTPipeline::my_bus_callback (GstBus     *bus,
				       GstMessage *message,
				       gpointer    data){
  std::string s = GST_MESSAGE_TYPE_NAME (message);
  GSTPipeline *self = static_cast<GSTPipeline*>(data);
  
  std::cerr <<  "Got " + s  + " message\n";
  
  switch (GST_MESSAGE_TYPE (message)) {
  case GST_MESSAGE_ERROR: {
    GError *err;
    gchar *debug;
    
    gst_message_parse_error (message, &err, &debug);
    s = err->message;
    std::cerr << "Error: " + s  + "\n";
    g_error_free (err);
    g_free (debug);

    break;
  }
  case GST_MESSAGE_EOS:
    /* end-of-stream */
    //g_main_loop_quit (self->loop);
    /*segfault sometimes*/
 // gst_element_set_state (GST_ELEMENT(self->pipeline), GST_STATE_NULL);
//     gst_element_set_state (GST_ELEMENT(self->pipeline), GST_STATE_PLAYING);
    break;
  default:
    /* unhandled message */
    break;
  }
  
  /* we want to be notified again the next time there is a message
   * on the bus, so returning TRUE (FALSE means we want to stop watching
   * for messages on the bus and our callback should not be called again)
   */
  return TRUE;
}

}
