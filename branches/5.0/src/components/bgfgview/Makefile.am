if ENABLE_COMPONENT_BGFGVIEW
bin_PROGRAMS = bgfgview
endif

bgfgview_SOURCES = bgmodelfactory.cpp \
			controller.cpp \
			main.cpp  \
			mainthread.cpp \
			model.cpp \
			viewgtk.cpp

bgfgview_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) \
			$(GTKMM_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) \
			-DGLADE_DIR=\""$(gladedir)"\"
bgfgview_LDFLAGS = $(JDEROBOTICE_LDFLAGS) $(GTKMM_LDFLAGS) $(COLORSPACESMM_LDFLAGS)
bgfgview_LDADD = $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/jderobotice/libJderobotIce.la

#headers will not be installed but they will be distributed
noinst_HEADERS = bgmodelfactory.h \
			controller.h \
			mainthread.h \
			model.h \
			viewgtk.h \
			widget.h

dist_conf_DATA = bgfgview.cfg
dist_glade_DATA = bgfgview.glade

MAINTAINERCLEANFILES = Makefile.in Makefile