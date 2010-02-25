#include "gstpipeline.h"
#include <iostream>


namespace cameraserver {

  Config::Config()
    :  name(""), 
       uri(""),
	width(0),
	height(0),
	bpp(0),
	framerateN(0),
	framerateD(0),
	format()
  {
  }

  bool
  Config::validate() const
  {
    if ( name == "") return false;
    if ( uri == "" ) return false;
    if ( width <= 0 ) return false;
    if ( height <= 0 ) return false;
    if ( bpp <= 0 ) return false;
    if ( framerateN <= 0 ) return false;
    if ( framerateD <= 0 ) return false;
    if ( !format ) return false;
    return true;
  }

  std::string
  Config::toString() const
  {
    std::stringstream ss;
    ss << "GSTVideoPipeline config: name= " << name << " uri=" << uri << 
      " width=" << width << " height=" << height << 
      " bpp=" << bpp << " framerate=" << framerateN << "/" << framerateD << 
      " format=" << format;
    return ss.str();
  }

  bool 
  Config::operator==( const Config & other )
  {
    return (name==other.name && uri==other.uri && width==other.width && 
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
    GError *err;
    if (g_thread_supported())
      g_thread_init(NULL);
    return gst_init_check(argcp,argvp,&err);
  }

  GSTPipeline::GSTPipeline(const jderobotice::Context& context, const Config &cfg) throw(jderobotice::ConfigFileException)
    : gbxiceutilacfr::SafeThread(context.tracer()),
      config_(cfg),context(context),mainloop(g_main_loop_new (NULL, FALSE)) {
    GstCaps *caps,*v4l2caps;
    colorspaces::Image::FormatPtr fmt;
  
    /* create pipeline, add handler */
    pipeline = gst_pipeline_new (cfg.name.c_str());
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, GSTPipeline::my_bus_cb, (void*)this);
    gst_object_unref (bus);

    /*fixed caps for v4l2 sources. ffmpegcolorspace negociate I420 if no caps 
      set but webcams produce YUV2, so extra translations are perfomed. 
      To avoid this we set the caps to force the format between src->videocolor*/
    v4l2caps = gst_caps_new_simple ("video/x-raw-yuv",
				    "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC('Y','U','Y','2'),
				    NULL);

    if (config_.format == colorspaces::ImageRGB888::FORMAT_RGB888){
      caps = gst_caps_new_simple ("video/x-raw-rgb",
				  "bpp", G_TYPE_INT,(8 * config_.format->bytesPerPixel()),
				  "depth",G_TYPE_INT,(8 * config_.format->bytesPerPixel()),
				  "width", G_TYPE_INT, config_.width,
				  "height", G_TYPE_INT, config_.height,
				  "framerate", GST_TYPE_FRACTION, 
				  config_.framerateN, config_.framerateD,
				  NULL);
    }else if (config_.format == colorspaces::ImageYUY2::FORMAT_YUY2){/*or FORMAT_YUYV*/
      caps = gst_caps_new_simple ("video/x-raw-yuv",
				  "format", GST_TYPE_FOURCC, GST_STR_FOURCC(config_.format->name.c_str()),
				  "width", G_TYPE_INT, config_.width,
				  "height", G_TYPE_INT, config_.height,
				  "framerate", GST_TYPE_FRACTION, 
				  config_.framerateN, config_.framerateD,
				  NULL);
    }else{
      throw jderobotice::ConfigFileException(ERROR_INFO, "Format " + config_.format->name + " is not supported within this server");
    }

    std::string capsStr(gst_caps_to_string(caps));
    context.tracer().info("Pipeline caps: " + capsStr);

    if (config_.uri.find("v4l2://") == 0){/*handle v4l source*/
      std::string dev = config_.uri.substr(7);/*after v4l://*/
      source = gst_element_factory_make("v4l2src","source");
      g_object_set(G_OBJECT(source),"device",dev.c_str(),NULL);
      sink = gst_element_factory_make("appsink","sink");
      g_object_set(G_OBJECT(sink),"drop",1,NULL);
      g_object_set(G_OBJECT(sink),"max-buffers",16,NULL);

      videocolor = gst_element_factory_make("ffmpegcolorspace","videocolor");
      gst_bin_add_many(GST_BIN(pipeline),source,videocolor,sink,NULL);
      gst_element_link_filtered(source,videocolor,v4l2caps);
      gst_element_link_filtered(videocolor,sink,caps);
    }else if(config_.uri.find("videotest://") == 0){/*handle videotest source*/
      std::string patternStr = config_.uri.substr(12);/*after videotest://*/
      std::stringstream s(patternStr);
      int pattern;

      source = gst_element_factory_make("videotestsrc","source");
      s >> pattern;/*FIXME: no error checked*/
      g_object_set(G_OBJECT(source),"pattern",pattern,NULL);
      sink = gst_element_factory_make("appsink","sink");

      g_object_set(G_OBJECT(sink),"drop",1,NULL);
      g_object_set(G_OBJECT(sink),"max-buffers",16,NULL);

      gst_bin_add_many(GST_BIN(pipeline),source,sink,NULL);
      gst_element_link_filtered(source,sink,caps);
    }else{
      source = gst_element_make_from_uri(GST_URI_SRC,config_.uri.c_str(),
					 "source");
      decoder = gst_element_factory_make("decodebin","decoder");
      g_signal_connect (decoder, "new-decoded-pad", G_CALLBACK (newpad_cb), (void*)this);
  
      videoscale = gst_element_factory_make("videoscale","videoscale");
      videorate = gst_element_factory_make("videorate","videorate");
      videocolor = gst_element_factory_make("ffmpegcolorspace","videocolor");
      sink = gst_element_factory_make("appsink","sink");

      g_object_set(G_OBJECT(sink),"drop",1,NULL);
      g_object_set(G_OBJECT(sink),"max-buffers",16,NULL);
      gst_bin_add_many(GST_BIN(pipeline),source,decoder,videorate,videoscale,videocolor,sink,NULL);
  
      gst_element_link(source,decoder);
      gst_element_link(videorate,videoscale);
      gst_element_link(videoscale,videocolor);
      gst_element_link_filtered(videocolor,sink,caps);
    }
  
    gst_caps_unref (caps);
    gst_caps_unref ( v4l2caps );
  }
  
  GSTPipeline::~GSTPipeline(){
    //gst_element_set_state (GST_ELEMENT(pipeline), GST_STATE_NULL);
    gbxiceutilacfr::stopAndJoin(this);

    gst_object_unref (pipeline);
    g_main_loop_unref (mainloop);
  }

  GstBuffer* GSTPipeline::pull_buffer(){
    return gst_app_sink_pull_buffer(GST_APP_SINK(sink));
  }

  void GSTPipeline::stop(){
    gst_element_set_state (GST_ELEMENT(pipeline), GST_STATE_NULL);
    if (g_main_loop_is_running(mainloop))
      g_main_loop_quit(mainloop);
  }

  void GSTPipeline::walk(){
    gst_element_set_state (GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    while(!isStopping()){
      g_main_loop_run (mainloop);
    }
  }

  void
  GSTPipeline::newpad_cb(GstElement *decode,
			 GstPad     *pad,
			 gboolean    last,
			 gpointer    data){
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

  gboolean GSTPipeline::my_bus_cb(GstBus     *bus,
				  GstMessage *message,
				  gpointer    data){
    std::string s = GST_MESSAGE_TYPE_NAME (message);
    GSTPipeline *self = static_cast<GSTPipeline*>(data);
  
    self->context.tracer().debug("Bus callback received message: " + s);

    switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR: {
      GError *err;
      gchar *debug;
    
      gst_message_parse_error (message, &err, &debug);
      s = err->message;
      self->context.tracer().error("Error: " + s);
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

}//namespace
