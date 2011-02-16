if ENABLE_COMPONENT_GIRAFFECLIENT
bin_PROGRAMS = giraffeclient
endif

giraffeclient_SOURCES = giraffeclient.cpp giraffeclient.h controller.cpp controller.h view.cpp view.h cameraconf.cpp cameraconf.h
giraffeclient_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) $(GTKMM_CPPFLAGS) $(GTKGLEXTMM_CPPFLAGS) $(OPENCV_CPPFLAGS) $(GSL_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
giraffeclient_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(GTKMM_LDFLAGS) $(GTKGLEXTMM_LDFLAGS) $(OPENCV_LDFLAGS) $(GSL_LDFLAGS)
giraffeclient_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/progeo/libprogeo.la

dist_conf_DATA = giraffeclient.cfg
dist_glade_DATA = giraffeclient.glade
