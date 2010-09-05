if ENABLE_LIBBGFGFILTERS
lib_LTLIBRARIES = libbgfgfilters.la
endif

libbgfgfilters_ladir = $(includedir)/bgfgfilters
libbgfgfilters_la_SOURCES = bgfg_common.cpp \
				bgfg_exp.cpp \
				bgfg_mean.cpp \
				bgfg_mode.cpp \
				bgfg_codebook.cpp
libbgfgfilters_la_CPPFLAGS = $(OPENCV_CPPFLAGS)
libbgfgfilters_la_LDFLAGS = $(OPENCV_LDFLAGS)
libbgfgfilters_la_HEADERS = bgfgfilters.h