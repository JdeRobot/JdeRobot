if ENABLE_COMPONENT_CAMERAVIEW_ICESTORM
bin_PROGRAMS = cameraview_icestorm
endif

cameraview_icestorm_SOURCES = cameraview_icestorm.cpp \
			viewer.cpp \
			viewer.h
cameraview_icestorm_CPPFLAGS = -I$(top_srcdir)/src/libs -I$(top_srcdir)/src/interfaces/cpp \
			$(GTKMM_CPPFLAGS) $(LIBCOLORSPACESMM_CPPFLAGS) \
			-DGLADE_DIR=\""$(gladedir)"\"
cameraview_icestorm_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(LIBICEBOX) $(LIBICESTORM) $(LIBICESTORMSERVICE) $(GTKMM_LDFLAGS) $(LIBCOLORSPACESMM_LDFLAGS)
cameraview_icestorm_LDADD = $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = cameraview_icestorm.cfg
dist_glade_DATA = cameraview.glade
