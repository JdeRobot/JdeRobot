bin_PROGRAMS = varcolorserver

#FIXME: move to configure
GSTREAMER_CFLAGS=`pkg-config --cflags gstreamer-0.10 gstreamer-app-0.10`
GSTREAMER_LIBS=`pkg-config --libs gstreamer-0.10 gstreamer-app-0.10`

varcolorserver_SOURCES = varcolorserver.cpp gstpipeline.cpp gstpipeline.h
varcolorserver_CXXFLAGS = $(GSTREAMER_CFLAGS) -I$(top_srcdir)/libs/colorspaces -I$(top_srcdir)/interfaces/cpp
varcolorserver_LDFLAGS = $(GSTREAMER_LIBS) -lIce -lIceUtil
varcolorserver_LDADD = $(top_srcdir)/libs/colorspaces/libcolorspaces.la \
			$(top_srcdir)/interfaces/cpp/jderobot/libjderobotice.la

dist_conf_DATA = varcolorserver.cfg