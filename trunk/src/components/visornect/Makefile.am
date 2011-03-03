if ENABLE_COMPONENT_VISORNECT
bin_PROGRAMS = visornect
endif

visornect_SOURCES = drawarea.cpp  drawarea.h  lambecom.cfg  Makefile.am  visornect.cfg  visornect.cpp  visornectgui.cpp  visornectgui.glade  visornectgui.h
visornect_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(GTKMM_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) $(GTKGLEXTMM_CPPFLAGS) $(OPENGL_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
visornect_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(GTKMM_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(GTKGLEXTMM_LDFLAGS) $(OPENGL_LDFLAGS)
visornect_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/progeo/libprogeo.la \
			$(top_srcdir)/src/libs/pioneer/libpioneer.la

dist_conf_DATA = visornect.cfg
dist_glade_DATA = visornectgui.glade
