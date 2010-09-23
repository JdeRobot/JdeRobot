if ENABLE_LIBBGFGSEGMENTATION
lib_LTLIBRARIES = libbgfgsegmentation.la
endif

libbgfgsegmentation_ladir = $(includedir)/bgfgsegmentation
libbgfgsegmentation_la_SOURCES = bgfg_common.cpp \
				bgfg_exp.cpp \
				bgfg_mean.cpp \
				bgfg_mode.cpp \
				bgfg_codebook.cpp \
				bgfg_fixed.cpp
libbgfgsegmentation_la_CPPFLAGS = $(OPENCV_CPPFLAGS)
libbgfgsegmentation_la_LDFLAGS = $(OPENCV_LDFLAGS)
libbgfgsegmentation_la_HEADERS = bgfgsegmentation.h
