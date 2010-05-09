if ENABLE_EVI
driver_LTLIBRARIES = evi.la
endif

EVILIB_DIR = EVILib
EVILIB_SOURCES = $(EVILIB_DIR)/EVI-D100.c++ \
		$(EVILIB_DIR)/EVI-D100g.c++ \
		$(EVILIB_DIR)/EVI-D100g.h \
		$(EVILIB_DIR)/EVI-D100.h \
		$(EVILIB_DIR)/EVI-D100P.c++ \
		$(EVILIB_DIR)/EVI-D100P.h \
		$(EVILIB_DIR)/EVI-D30.c++ \
		$(EVILIB_DIR)/EVI-D30g.c++ \
		$(EVILIB_DIR)/EVI-D30g.h \
		$(EVILIB_DIR)/EVI-D30.h \
		$(EVILIB_DIR)/EVI-D31.c++ \
		$(EVILIB_DIR)/EVI-D31.h \
		$(EVILIB_DIR)/EVI-D70.c++ \
		$(EVILIB_DIR)/EVI-D70g.c++ \
		$(EVILIB_DIR)/EVI-D70g.h \
		$(EVILIB_DIR)/EVI-D70.h \
		$(EVILIB_DIR)/EVI-D70P.c++ \
		$(EVILIB_DIR)/EVI-D70P.h \
		$(EVILIB_DIR)/EVILib.c++ \
		$(EVILIB_DIR)/EVILib.h

evi_la_SOURCES = $(EVILIB_SOURCES) evi.cpp camera.cpp evi.h camera.h
evi_la_LDFLAGS = -module -avoid-version $(EVI_LIBS) $(DRIVER_LIBS) $(DRIVER_LDFLAGS)
evi_la_CXXFLAGS = $(EVI_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir)/base
dist_conf_DATA = evi.conf


MAINTAINERCLEANFILES = Makefile.in Makefile
