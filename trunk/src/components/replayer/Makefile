JDEROBOTDIR = /usr/local
CXXFLAGS = -I $(JDEROBOTDIR)/include/jderobot -I /usr/include -I /usr/include/gearbox `pkg-config --cflags opencv playerc` `pkg-config --cflags gtkmm-2.4 libglademm-2.4 gthread-2.0` `pkg-config --cflags gtkglextmm-1.2`
LDFLAGS = `pkg-config --libs gtkmm-2.4 libglademm-2.4 gthread-2.0 playerc` `pkg-config --libs opencv` -lgsl -lgslcblas `pkg-config --libs gtkglextmm-1.2` -lGL -lGLU -lglut -lgazebo

LDADD = $(JDEROBOTDIR)/lib/jderobot/libJderobotIce.la \
	$(JDEROBOTDIR)/lib/jderobot/libJderobotUtil.la \
	$(JDEROBOTDIR)/lib/jderobot/libcolorspacesmm.la \
	$(JDEROBOTDIR)/lib/jderobot/libJderobotInterfaces.la 

playerserver: replayer.o kinect.o 
	libtool --mode=link g++ -g -O -o replayer replayer.o kinect.o  $(LDADD) $(LDFLAGS) 
