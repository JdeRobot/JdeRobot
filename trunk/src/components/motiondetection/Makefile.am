if ENABLE_COMPONENT_MOTIONDETECTION
bin_PROGRAMS = motiondetection
lib_LTLIBRARIES = libmotiondetection.la
endif

libmotiondetection_la_SOURCES = motiondetection.cpp motiondetection.h \
				view.cpp view.h model.cpp model.h controller.cpp controller.h
libmotiondetection_la_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(GTKMM_CPPFLAGS) $(OPENCV_CPPFLAGS) \
				-DGLADE_DIR=\""$(gladedir)"\"
libmotiondetection_la_LIBADD = $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
				$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la
libmotiondetection_la_LDFLAGS = $(JDEROBOTICE_LDFLAGS) $(OPENCV_LDFLAGS) $(GTKMM_LDFLAGS)

motiondetection_SOURCES = main.cpp
motiondetection_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) \
			$(libmotiondetection_la_CPPFLAGS)
#motiondetection_LDFLAGS = $(libmotiondetection_la_LDFLAGS)
motiondetection_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			libmotiondetection.la

dist_conf_DATA = motiondetection.cfg
dist_glade_DATA = motiondetection.glade
MAINTAINERCLEANFILES = Makefile.in Makefile