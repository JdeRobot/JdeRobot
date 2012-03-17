JDEROBOTDIR = /usr/local
CXXFLAGS = -I/usr/local/include/gazebo -I$(JDEROBOTDIR)/include/jderobot -I$(JDEROBOTDIR)/include/jderobot/progeo -I/usr/include/gearbox `pkg-config --cflags opencv` `pkg-config --cflags gtkmm-2.4 libglademm-2.4 gthread-2.0` `pkg-config --cflags gtkglextmm-1.2` -I/usr/include/pcl-1.5 -I/usr/include/openni  -I/usr/include/libusb-1.0   -I/usr/include/cminpack-1 -I/usr/include/eigen3 -I/usr/include/vtk-5.2/ 
LDFLAGS = `pkg-config --libs gtkmm-2.4 libglademm-2.4 gthread-2.0` `pkg-config --libs opencv` -lgsl -lgslcblas `pkg-config --libs gtkglextmm-1.2` -lGL -lGLU -lglut -lgazebo -lpcl_visualization -lpcl_surface -lpcl_registration -lpcl_segmentation -lpcl_filters -lpcl_sample_consensus \
-lpcl_io -lm -lpcl_octree -lOpenNI -lusb-1.0 -lpcl_features -lpcl_kdtree -lpcl_common 
LDADD = $(JDEROBOTDIR)/lib/jderobot/libJderobotIce.la \
	$(JDEROBOTDIR)/lib/jderobot/libJderobotUtil.la \
	$(JDEROBOTDIR)/lib/jderobot/libcolorspacesmm.la \
	$(JDEROBOTDIR)/lib/jderobot/libJderobotInterfaces.la \
	$(JDEROBOTDIR)/lib/jderobot/libprogeo.la \
	$(JDEROBOTDIR)/lib/jderobot/libpioneer.la

gazeboserver: kinectServer.o cloudPoints.o
	libtool --mode=link g++ -g -O -o kinectServer kinectServer.o cloudPoints.o $(LDADD) $(LDFLAGS) -Wno-deprecated
