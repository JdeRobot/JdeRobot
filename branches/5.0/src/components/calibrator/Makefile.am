if ENABLE_COMPONENT_CALIBRATOR
bin_PROGRAMS = calibrator
endif

calibrator_SOURCES = calibratorgui.cpp calibratorgui.h libcalibrator.cpp libcalibrator.h calibrator.cpp

calibrator_CPPFLAGS = -I$(top_srcdir)/src/libs -I$(top_srcdir)/src/interfaces/cpp $(LIBCOLORSPACESMM_CPPFLAGS) 

calibrator_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(LIBCOLORSPACESMM_LDFLAGS) $(LIBX11) $(LIBFORMS) 

calibrator_LDADD = $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/progeo/libprogeo.la

dist_conf_DATA = calibrator.cfg
