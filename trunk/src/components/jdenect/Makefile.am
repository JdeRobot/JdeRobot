if ENABLE_COMPONENT_JDENECT
bin_PROGRAMS = jdenect
endif

jdenect_SOURCES = controller.cpp controller.h jdenect.cfg jdenect.cpp libfreenect.hpp
jdenect_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(GTKMM_CPPFLAGS) $(GTKGLEXTMM_CPPFLAGS) $(OPENGL_CPPFLAGS) $(GSL_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) $(OPENCV_CPPFLAGS) $(FREENECT_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
jdenect_LDFLAGS = $(JDEROBOTICE_LDFLAGS) $(GTKMM_LDFLAGS) $(GTKGLEXTMM_LDFLAGS) $(OPENGL_LDFLAGS) $(GSL_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(OPENCV_LDFLAGS) $(FREENECT_LDFLAGS)
jdenect_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = jdenect.cfg
