if ENABLE_LIBBGFGSEGMENTATION
lib_LTLIBRARIES = libbgfgsegmentation.la
endif

libbgfgsegmentation_ladir = $(includedir)/bgfgsegmentation
libbgfgsegmentation_la_SOURCES = bgfg_common.cpp \
				bgfg_exp.cpp \
				bgfg_mean.cpp \
				bgfg_mode.cpp \
				bgfg_codebook.cpp \
				bgfg_fixed.cpp \
				bgmodelfactory.cpp
libbgfgsegmentation_la_CPPFLAGS = $(GEARBOX_CPPFLAGS) $(OPENCV_CPPFLAGS) -I${top_srcdir}/src/libs
libbgfgsegmentation_la_LDFLAGS = $(GEARBOX_LDFLAGS) $(OPENCV_LDFLAGS) $(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la
libbgfgsegmentation_la_HEADERS = bgfgsegmentation.h bgmodelfactory.h
