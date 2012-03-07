CXXFLAGS = -I $(JDEROBOTDIR)/include/jderobot -I. -I/usr/include/gearbox/ `pkg-config --cflags opencv` `pkg-config --cflags gtkmm-2.4 libglademm-2.4 gthread-2.0` -I/usr/include/nite -I/usr/include/openni -I../Bin -I/usr/include/ni/
LDFLAGS = `pkg-config --libs gtkmm-2.4 libglademm-2.4 gthread-2.0` `pkg-config --libs opencv` -lgsl -lgslcblas -lglut -lOpenNI
LDADD = $(JDEROBOTDIR)/lib/jderobot/libJderobotIce.la \
        $(JDEROBOTDIR)/lib/jderobot/libJderobotUtil.la \
        $(JDEROBOTDIR)/lib/jderobot/libcolorspacesmm.la \
        $(JDEROBOTDIR)/lib/jderobot/libJderobotInterfaces.la \
        $(JDEROBOTDIR)/lib/jderobot/libprogeo.la

openniServer: openniServer.o
	libtool --mode=link g++ -g -O -malign-double -O3 -fno-tree-pre -fno-strict-aliasing -DNDEBUG -msse3 -mssse3 -DUSE_GLUT -DXN_SSE -o openniServer openniServer.o $(LDADD) $(LDFLAGS)

clean:
	rm -rf *.o openniServer *.gch
