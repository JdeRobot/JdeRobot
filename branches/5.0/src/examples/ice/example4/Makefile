CXXFLAGS=-Wall -I. -g

GSTREAMER_CFLAGS=`pkg-config --cflags gstreamer-0.10 gstreamer-app-0.10`
GSTREAMER_LIBS=`pkg-config --libs gstreamer-0.10 gstreamer-app-0.10`

GTKMM_CFLAGS=`pkg-config --cflags gtkmm-2.4 libglademm-2.4 gthread-2.0`
GTKMM_LIBS=`pkg-config --libs gtkmm-2.4 libglademm-2.4 gthread-2.0`

%.h: %.ice
	slice2cpp -I. $<

all: ice2cpp varcolorserver varcolorviewgtkmm

ice2cpp: jdecommon.h varcolor.h

varcolorserver: varcolorserver.cpp gstpipeline.cpp jdecommon.cpp varcolor.cpp
	g++ $(CXXFLAGS) -lIce -lIceUtil $(GSTREAMER_CFLAGS) $(GSTREAMER_LIBS) -o $@ $^

varcolorviewgtkmm: varcolorviewgtkmm.cpp viewer.cpp jdecommon.cpp varcolor.cpp
	g++ $(CXXFLAGS) -lm -lIce -lIceUtil $(GTKMM_CFLAGS) $(GTKMM_LIBS) -o $@ $^

clean:
	rm -f varcolorserver varcolorviewgtkmm *.o varcolor.h varcolor.cpp jdecommon.h jdecommon.cpp

.PHONY: all ice2cpp clean