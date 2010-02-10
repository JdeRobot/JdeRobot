if ENABLE_COMPONENT_OPENCVDEMO
bin_PROGRAMS = opencvdemo
endif

opencvdemo_SOURCES = opencvdemo.cpp \
			view.cpp \
			view.h \
			model.cpp \
			model.h
opencvdemo_CPPFLAGS =  $(GTKMM_CPPFLAGS) $(OPENCV_CPPFLAGS) \
			$(LIBJDEROBOTICE_CPPFLAGS) \
			-I$(top_srcdir)/src/libs/colorspaces \
			-I$(top_srcdir)/src/interfaces/cpp -DGLADE_DIR=\""$(gladedir)"\"
opencvdemo_LDFLAGS = $(GTKMM_LDFLAGS) $(OPENCV_LDFLAGS)
opencvdemo_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspaces.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = opencvdemo.cfg
dist_glade_DATA = opencvdemo.glade