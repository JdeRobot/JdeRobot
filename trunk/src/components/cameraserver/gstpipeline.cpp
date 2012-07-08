#include "gstpipeline.h"
#include <iostream>


namespace cameraserver {

  Config::Config()
    : name(""),
      uri(""),
      width(0),
      height(0),
      bpp(0),
      framerateN(0),
      framerateD(0),
      format() {
  }

  bool
  Config::validate() const {
    if (name == "") return false;
    if (srcpipeline == "" || uri == "") return false;
    if (width <= 0) return false;
    if (height <= 0) return false;
    if (bpp <= 0) return false;
    if (framerateN <= 0) return false;
    if (framerateD <= 0) return false;
    if (!format) return false;
    return true;
  }

  std::string
  Config::toString() const {
    std::stringstream ss;
    ss << "GSTVideoPipeline config: name=" << name << " srcpipeline=" <<  srcpipeline << " uri=" << uri <<
      " width=" << width << " height=" << height <<
      " bpp=" << bpp << " framerate=" << framerateN << "/" << framerateD <<
      " format=" << format;
    return ss.str();
  }

  bool
  Config::operator==(const Config & other) {
    return (name == other.name && srcpipeline == other.srcpipeline && uri == other.uri && width == other.width &&
	    height == other.height && bpp == other.bpp &&
	    framerateN == other.framerateN && framerateD == other.framerateD &&
	    format == other.format);
  }

  bool
  Config::operator!=(const Config & other) {
    return !(*this == other);
  }

  int gst_init(int* argcp, char** argvp[]) {
    GError *err;
    if (g_thread_supported())
      g_thread_init(NULL);
    return gst_init_check(argcp, argvp, &err);
  }

  const std::string GSTPipeline::v4l2UriScheme = "v4l2://";
  const std::string GSTPipeline::v4lUriScheme = "v4l://";
  const std::string GSTPipeline::videotestUriScheme = "videotest://";

  GSTPipeline::GSTPipeline(const jderobotice::Context& context, const Config &cfg) throw (jderobotice::ConfigFileException)
    : gbxiceutilacfr::SafeThread(context.tracer()),
      config_(cfg), context(context), mainloop(g_main_loop_new(NULL, FALSE)),eos(false) {
    pipeline = build_pipeline(cfg);
    context.tracer().info("Starting pipeline");
    start(); //my own thread
  }

  GSTPipeline::~GSTPipeline() {
    //gst_element_set_state (GST_ELEMENT(pipeline), GST_STATE_NULL);
    gbxiceutilacfr::stopAndJoin(this);

    gst_object_unref(pipeline);
    g_main_loop_unref(mainloop);
  }

  GstElement* GSTPipeline::build_pipeline(const Config &cfg) throw (jderobotice::ConfigFileException){
    GstCaps *caps;
    GstElement *p;
    GstBus *bus;

    /* create pipeline, add handler */
    p = gst_pipeline_new(cfg.name.c_str());
    bus = gst_pipeline_get_bus(GST_PIPELINE(p));
    gst_bus_add_watch(bus, GSTPipeline::my_bus_cb, (void*) this);
    gst_object_unref(bus);

    if (config_.format == colorspaces::ImageRGB8::FORMAT_RGB8) {
      caps = gst_caps_new_simple("video/x-raw-rgb",
				 "bpp", G_TYPE_INT, (8 * config_.format->bytesPerPixel()),
				 "depth", G_TYPE_INT, (8 * config_.format->bytesPerPixel()),
				 "width", G_TYPE_INT, config_.width,
				 "height", G_TYPE_INT, config_.height,
				 "framerate", GST_TYPE_FRACTION,
				 config_.framerateN, config_.framerateD,
				 NULL);
    } else if (config_.format == colorspaces::ImageYUY2::FORMAT_YUY2) {/*or FORMAT_YUYV*/
      caps = gst_caps_new_simple("video/x-raw-yuv",
				 "format", GST_TYPE_FOURCC, GST_STR_FOURCC(config_.format->name.c_str()),
				 "width", G_TYPE_INT, config_.width,
				 "height", G_TYPE_INT, config_.height,
				 "framerate", GST_TYPE_FRACTION,
				 config_.framerateN, config_.framerateD,
				 NULL);
    } else {
      throw jderobotice::ConfigFileException(ERROR_INFO, "Format " + config_.format->name + " is not supported within this server");
    }

    std::string capsStr(gst_caps_to_string(caps));
    context.tracer().info("Pipeline caps: " + capsStr);

    if (config_.srcpipeline != ""){//if pipeline supplied is prefered
      GError *err = 0;
      std::string srcpipeline(config_.srcpipeline+"! identity ");//append identity element to allow partial pipelines
      source = gst_parse_bin_from_description(srcpipeline.c_str(),1,&err);//ghost pads are linked
      if (err!=0){
	std::string err_msg(err->message);
	g_error_free(err);
	throw jderobotice::ConfigFileException(ERROR_INFO, "Can't build requested pipeline: " + err_msg);
      }
    }else if (config_.uri.find(v4l2UriScheme) == 0) {/*handle v4l2 source*/
      std::string dev = config_.uri.substr(v4l2UriScheme.size()); /*after v4l2://*/
      source = gst_element_factory_make("v4l2src", "source");
      g_object_set(G_OBJECT(source), "device", dev.c_str(), NULL);
    }else if (config_.uri.find(v4lUriScheme) == 0) {/*handle v4l source*/
      std::string dev = config_.uri.substr(v4lUriScheme.size()); /*after v4l://*/
      source = gst_element_factory_make("v4lsrc", "source");
      g_object_set(G_OBJECT(source), "device", dev.c_str(), NULL);
    } else if (config_.uri.find(videotestUriScheme) == 0) {/*handle videotest source*/
      std::string patternStr = config_.uri.substr(videotestUriScheme.size()); /*after videotest://*/
      std::stringstream s(patternStr);
      int pattern;
      source = gst_element_factory_make("videotestsrc", "source");
      s >> pattern; /*FIXME: no error checked*/
      g_object_set(G_OBJECT(source), "pattern", pattern, NULL);
    } else {
      GstElement *urisrc = gst_element_make_from_uri(GST_URI_SRC, config_.uri.c_str(), "urisrc");
      GstElement *decoder = gst_element_factory_make("decodebin", "decoder");
      g_signal_connect(decoder, "new-decoded-pad", G_CALLBACK(newpad_cb), (void*) this);

      source = gst_bin_new("source");
      gst_bin_add_many(GST_BIN(source),urisrc,decoder,NULL);
      gst_element_link(urisrc, decoder);
      //we create the bin src ghost pad without target because decode
      //will come with it when playing. See newpad_cb callback
      gst_element_add_pad (source, gst_ghost_pad_new_no_target ("src", GST_PAD_SRC));
    }

    gst_bin_add(GST_BIN(p), source);
    videoscale = gst_element_factory_make("videoscale", "videoscale");
    videorate = gst_element_factory_make("videorate", "videorate");
    videocolor = gst_element_factory_make("ffmpegcolorspace", "videocolor");
    sink = gst_element_factory_make("appsink", "sink");

    g_object_set(G_OBJECT(sink), "drop", 1, NULL);
    g_object_set(G_OBJECT(sink), "max-buffers", 16, NULL);
    gst_bin_add_many(GST_BIN(p), videorate, videoscale, videocolor, sink, NULL);

    gst_element_link(source,videorate);
    gst_element_link(videorate, videoscale);
    gst_element_link(videoscale, videocolor);
    gst_element_link_filtered(videocolor, sink, caps);

    gst_caps_unref(caps);
    return p;
  }

  GstBuffer* GSTPipeline::pull_buffer() {
    GstBuffer* buff = 0;
    if (!gst_app_sink_is_eos(GST_APP_SINK(sink)))
      buff = gst_app_sink_pull_buffer(GST_APP_SINK(sink));
    return buff;
  }

  bool GSTPipeline::isEos(){
    return eos;
  }

  void GSTPipeline::stop() {
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
    if (g_main_loop_is_running(mainloop))
      g_main_loop_quit(mainloop);
  }

  void GSTPipeline::walk() {
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    while (!isStopping()) {
      g_main_loop_run(mainloop);
    }
  }

  void
  GSTPipeline::newpad_cb(GstElement *decode,
			 GstPad *pad,
			 gboolean last,
			 gpointer data) {
    GstCaps *caps = 0;
    GstStructure *str;
    GstPad *gpad = 0;
    GstPad *target = 0;
    GSTPipeline *self = static_cast<GSTPipeline*> (data);
    GstElement *source = self->source;

    gpad = gst_element_get_static_pad(source, "src");
    target = gst_ghost_pad_get_target(GST_GHOST_PAD(gpad));
    caps = gst_pad_get_caps(pad);
    str = gst_caps_get_structure(caps, 0);
    if (target == 0 && g_strrstr(gst_structure_get_name(str), "video"))//ghost pad without target && pad media type video
      gst_ghost_pad_set_target(GST_GHOST_PAD(gpad), pad);/* link'n'play */
    else
      gst_object_unref(pad);//unref pad, we won't use it

    gst_caps_unref(caps);
    gst_object_unref(gpad);
    if (target != 0)
      gst_object_unref(target);
  }

  gboolean GSTPipeline::my_bus_cb(GstBus *bus,
				  GstMessage *message,
				  gpointer data) {
    std::string s = GST_MESSAGE_TYPE_NAME(message);
    GSTPipeline *self = static_cast<GSTPipeline*> (data);

    self->context.tracer().debug("Bus callback received message: " + s);

    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_ERROR:
      {
	GError *err;
	gchar *debug;

	gst_message_parse_error(message, &err, &debug);
	s = err->message;
	self->context.tracer().error("Error: " + s);
	g_error_free(err);
	g_free(debug);
	gst_element_set_state (GST_ELEMENT(self->pipeline), GST_STATE_NULL);//app_sink_pull_buffer will quit
	g_main_loop_quit (self->mainloop);
	break;
      }
    // case GST_MESSAGE_STATE_CHANGED: {
    //   GstState old_state, new_state;
    
    //   gst_message_parse_state_changed (message, &old_state, &new_state, NULL);
    //   self->context.tracer().info("Element " + std::string(GST_OBJECT_NAME (message->src)) +
    // 				  " changed state from " + std::string(gst_element_state_get_name (old_state)) +
    // 				  " to " + std::string(gst_element_state_get_name (new_state)));
    //   break;
    // }
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      g_main_loop_quit (self->mainloop);
      self->eos = true;
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
