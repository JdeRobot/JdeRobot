if ENABLE_COMPONENT_INTROROB
bin_PROGRAMS = introrob
endif

introrob_SOURCES = introrob.cpp controller.cpp controller.h view.cpp view.h drawarea.cpp drawarea.h pioneer.c pioneer.h pioneeropengl.c pioneeropengl.h navega.cpp navega.h navegacion.cpp navegacion.h
introrob_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(GTKMM_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) $(GTKGLEXTMM_CPPFLAGS) $(OPENGL_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
introrob_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(GTKMM_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(GTKGLEXTMM_LDFLAGS) $(OPENGL_LDFLAGS)
introrob_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/progeo/libprogeo.la \
			$(top_srcdir)/src/libs/pioneer/libpioneer.la

dist_conf_DATA = introrob.cfg
dist_glade_DATA = introrob.glade
