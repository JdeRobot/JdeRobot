lib_LTLIBRARIES = libJderobotInterfaces.la

#could we build this list?
SLICESOURCES = camera.ice common.ice body.ice containers.ice datetime.ice exceptions.ice image.ice jcm.ice recording.ice varcolor.ice recorder.ice motors.ice laser.ice encoders.ice ptmotors.ice ptencoders.ice sonars.ice surveillance.ice bodyencoders.ice bodymotors.ice jointmotor.ice bodymovements.ice kinect.ice kinectleds.ice
SLICECPP_BUILT_CPP = $(SLICESOURCES:.ice=.cpp)
SLICECPP_BUILT_H = $(SLICESOURCES:.ice=.h)

#slice has to build all headers and sources from .ice files
BUILT_SOURCES = $(SLICECPP_BUILT_CPP) $(SLICECPP_BUILT_H)

#we don't distribute built cpp files
nodist_libJderobotInterfaces_la_SOURCES = $(SLICECPP_BUILT_CPP) 

libJderobotInterfaces_ladir = $(includedir)/jderobot
libJderobotInterfaces_la_LDFLAGS = $(ICE_LDFLAGS) -version-info 0:0:0
libJderobotInterfaces_la_CPPFLAGS = $(ICE_CPPFLAGS) -I..

#built headers will be distributed
libJderobotInterfaces_la_HEADERS = $(SLICECPP_BUILT_H)

#Clean all built sources
CLEANFILES = $(BUILT_SOURCES)

-include $(DEPDIR)/$(SLICESOURCES:.ice=.d) 

vpath %.ice $(SLICESRCDIR)/jderobot
.ice.d:
	@set -e; rm -f $@; \
	$(SLICE2CPP) --depend -I $(SLICESRCDIR) -I$(SLICEDIR) $< > $@.$$$$; \
	sed 's/\($*\)\.cpp[ :]*/\1.cpp $@: /g' < $@.$$$$ > $(DEPDIR)/$@; \
	rm -f $@.$$$$

.ice.cpp:
	$(SLICE2CPP) -I../../slice -I$(SLICEDIR) $<
