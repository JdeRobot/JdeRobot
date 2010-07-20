if ENABLE_COMPONENT_CALIBRATOR
bin_PROGRAMS = calibrator
endif

calibrator_SOURCES = libcalibrator.cpp libcalibrator.h calibrator.cpp calibratorgui.fd calibratorgui.c calibratorgui.h
calibrator_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) 
calibrator_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(LIBX11) $(LIBFORMS) 
calibrator_LDADD = $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/progeo/libprogeo.la

BUILT_SOURCES = calibratorgui.h
CLEANFILES = calibratorgui.c calibratorgui.h
calibratorgui.h: calibratorgui.fd
	$(FDESIGN) -convert calibratorgui.fd

dist_conf_DATA = calibrator.cfg
