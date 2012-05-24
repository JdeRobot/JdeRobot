if ENABLE_COMPONENT_TELEOPERATOR
bin_PROGRAMS = teleoperator
endif

teleoperator_SOURCES = teleoperator.cpp view.cpp controller.cpp JointMotor.cpp cameraConf.cpp drawarea.cpp pioneeropengl.c
teleoperator_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(GTKMM_CPPFLAGS) $(GTKGLEXTMM_CPPFLAGS) $(OPENGL_CPPFLAGS) $(GSL_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) $(OPENCV_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
teleoperator_LDFLAGS = $(JDEROBOTICE_LDFLAGS) $(GTKMM_LDFLAGS) $(GTKGLEXTMM_LDFLAGS) $(OPENGL_LDFLAGS) $(GSL_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(OPENCV_LDFLAGS)
teleoperator_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/progeo/libprogeo.la \
			$(top_srcdir)/src/libs/pioneer/libpioneer.la

dist_conf_DATA = teleoperator.cfg
dist_glade_DATA = teleoperatorgui.glade
