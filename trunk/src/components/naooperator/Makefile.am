if ENABLE_COMPONENT_NAOOPERATOR
bin_PROGRAMS = naooperator
endif

naooperator_SOURCES = naooperator.cpp naooperatorgui.cpp controllers/bodyencoders-controller.cpp controllers/bodymotors-controller.cpp controllers/motors-controller.cpp controllers/ptencoders-controller.cpp controllers/ptmotors-controller.cpp gui-modules/bodyencodersGui.cpp gui-modules/bodymotorsGui.cpp gui-modules/motorsGui.cpp gui-modules/ptencodersGui.cpp gui-modules/ptmotorsGui.cpp
naooperator_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(GTKMM_CPPFLAGS) $(GTKGLEXTMM_CPPFLAGS) $(OPENGL_CPPFLAGS) $(GSL_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) $(OPENCV_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
naooperator_LDFLAGS = $(JDEROBOTICE_LDFLAGS) $(GTKMM_LDFLAGS) $(GTKGLEXTMM_LDFLAGS) $(OPENGL_LDFLAGS) $(GSL_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(OPENCV_LDFLAGS)
naooperator_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = naooperator.cfg
dist_glade_DATA = naooperatorgui.glade
