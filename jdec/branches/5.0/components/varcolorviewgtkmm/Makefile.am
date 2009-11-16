bin_PROGRAMS = varcolorviewgtkmm

#FIXME: move to configure
GTKMM_CFLAGS=`pkg-config --cflags gtkmm-2.4 libglademm-2.4 gthread-2.0`
GTKMM_LIBS=`pkg-config --libs gtkmm-2.4 libglademm-2.4 gthread-2.0`

varcolorviewgtkmm_SOURCES = varcolorviewgtkmm.cpp \
				viewer.cpp \
				viewer.h
varcolorviewgtkmm_CXXFLAGS = $(GTKMM_CFLAGS) -I$(top_srcdir)/libs/colorspaces -I$(top_srcdir)/interfaces/cpp -DGLADE_DIR=$(gladedir)
varcolorviewgtkmm_LDFLAGS = $(GTKMM_LIBS) -lIce -lIceUtil
varcolorviewgtkmm_LDADD = $(top_srcdir)/libs/colorspaces/libcolorspaces.la \
			$(top_srcdir)/interfaces/cpp/jderobot/libjderobotice.la

dist_conf_DATA = varcolorviewgtkmm.cfg
dist_glade_DATA = varcolorviewgtkmm.glade