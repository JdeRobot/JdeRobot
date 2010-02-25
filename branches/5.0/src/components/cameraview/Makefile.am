if ENABLE_COMPONENT_CAMERAVIEW
bin_PROGRAMS = cameraview
endif

cameraview_SOURCES = cameraview.cpp \
			viewer.cpp \
			viewer.h
cameraview_CPPFLAGS = -I$(top_srcdir)/src/libs -I$(top_srcdir)/src/interfaces/cpp \
			$(GTKMM_CPPFLAGS) $(LIBCOLORSPACESMM_CPPFLAGS) \
			-DGLADE_DIR=\""$(gladedir)"\"
cameraview_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(GTKMM_LDFLAGS) $(LIBCOLORSPACESMM_LDFLAGS)
cameraview_LDADD = $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = cameraview.cfg
dist_glade_DATA = cameraview.glade