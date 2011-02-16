if ENABLE_COMPONENT_CALIBRATOR
bin_PROGRAMS = calibrator
endif

calibrator_SOURCES = calibrator.cpp controller.cpp controller.h view.cpp view.h
calibrator_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) $(GTKMM_CPPFLAGS) $(GTKGLEXTMM_CPPFLAGS) $(OPENCV_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
calibrator_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(GTKMM_LDFLAGS) $(GTKGLEXTMM_LDFLAGS) $(OPENCV_LDFLAGS)
calibrator_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/progeo/libprogeo.la

dist_conf_DATA = calibrator.cfg
dist_glade_DATA = calibrator.glade
