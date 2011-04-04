if ENABLE_COMPONENT_COLORTUNER
    bin_PROGRAMS = colortuner
endif

colortuner_SOURCES = colorTuner.cpp viewer.cpp 
colortuner_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) $(GTKMM_CPPFLAGS) $(GTKGLEXTMM_CPPFLAGS) $(OPENCV_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
colortuner_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(GTKMM_LDFLAGS) $(GTKGLEXTMM_LDFLAGS) $(OPENCV_LDFLAGS)
colortuner_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/progeo/libprogeo.la

dist_conf_DATA = colortuner.cfg
