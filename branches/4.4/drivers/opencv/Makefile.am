if ENABLE_OPENCV
driver_LTLIBRARIES = opencv.la
endif

opencv_la_SOURCES = opencv.c
opencv_la_LDFLAGS = -module -avoid-version $(DRIVER_LDFLAGS) $(DRIVER_LIBS) $(OPENCV_LIBS) -lcxcore -lcv -lhighgui -lcvaux -lml
opencv_la_CFLAGS = $(OPENCV_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base -I/usr/include/opencv
dist_conf_DATA = opencv.conf

MAINTAINERCLEANFILES = Makefile.in Makefile
