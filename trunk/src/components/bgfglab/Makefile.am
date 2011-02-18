if ENABLE_COMPONENT_BGFGLAB
bin_PROGRAMS = bgfglab
endif

bgfglab_SOURCES = controller.cpp \
			main.cpp  \
			mainthread.cpp \
			model.cpp \
			viewgtk.cpp \
			viewtext.cpp

bgfglab_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(JDEROBOTUTIL_CPPFLAGS) \
			$(GTKMM_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) \
			-DGLADE_DIR=\""$(gladedir)"\"
bgfglab_LDFLAGS = $(JDEROBOTICE_LDFLAGS) $(JDEROBOTUTIL_LDFLAGS) $(GTKMM_LDFLAGS) $(COLORSPACESMM_LDFLAGS)
bgfglab_LDADD = $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/libs/bgfgsegmentation/libbgfgsegmentation.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la

#headers will not be installed but they will be distributed
noinst_HEADERS = controller.h \
			mainthread.h \
			model.h \
			viewgtk.h \
			widget.h

dist_conf_DATA = bgfglab.cfg
dist_glade_DATA = bgfglab.glade

MAINTAINERCLEANFILES = Makefile.in Makefile
