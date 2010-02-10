lib_LTLIBRARIES = libJderobotInterfaces.la

SLICECPP_BUILT_CPP = jcm.cpp \
		common.cpp \
		containers.cpp \
		datetime.cpp \
		exceptions.cpp \
		varcolor.cpp \
		recording.cpp

SLICECPP_BUILT_H = jcm.h \
		common.h \
		containers.h \
		datetime.h \
		exceptions.h \
		varcolor.h \
		recording.h

#slice has to build all headers and sources from .ice files
BUILT_SOURCES = $(SLICECPP_BUILT_CPP) $(SLICECPP_BUILT_H)

#we don't distribute built cpp files
nodist_libJderobotInterfaces_la_SOURCES = $(SLICECPP_BUILT_CPP) 

libJderobotInterfaces_ladir = $(includedir)/jderobot
libJderobotInterfaces_la_LDFLAGS = $(LIBICE) -version-info 0:0:0
libJderobotInterfaces_la_CPPFLAGS = -I..

#built headers will be distributed
libJderobotInterfaces_la_HEADERS = $(SLICECPP_BUILT_H)

#Clean all built sources
CLEANFILES = $(BUILT_SOURCES)

#FIXME: generated with slice2cpp --depend <slice-file>
vpath %.ice ../../slice/jderobot
jcm.h: jcm.cpp
common.h: common.cpp
containers.h: containers.cpp 
datetime.h: datetime.cpp
exceptions.h: exceptions.cpp
varcolor.h: varcolor.cpp 
recording.h: recording.cpp
jcm.cpp: jcm.ice
common.cpp: common.ice datetime.ice exceptions.ice containers.ice
containers.cpp: containers.ice
datetime.cpp: datetime.ice
exceptions.cpp: exceptions.ice
varcolor.cpp: varcolor.ice common.ice datetime.ice exceptions.ice containers.ice
recording.cpp: recording.ice

.ice.cpp:
	$(SLICE2CPP) -I../../slice $<

