JDEROBOTDIR = /usr/local
CXXFLAGS = -Wall -g -I. -I$(JDEROBOTDIR)/include/jderobot `pkg-config --cflags opencv` `pkg-config --cflags gtkmm-2.4 libglademm-2.4 gthread-2.0` `pkg-config --cflags gtkglextmm-1.2`
LDFLAGS = `pkg-config --libs gtkmm-2.4 libglademm-2.4 gthread-2.0` `pkg-config --libs opencv` -lgsl -lgslcblas `pkg-config --libs gtkglextmm-1.2`
LDADD = $(JDEROBOTDIR)/lib/jderobot/libJderobotIce.so \
	$(JDEROBOTDIR)/lib/jderobot/libJderobotUtil.so \
	$(JDEROBOTDIR)/lib/jderobot/libJderobotInterfaces.so \
	$(JDEROBOTDIR)/lib/jderobot/libprogeo.so

all: visionlib.o geometry.o image.o linesDetection.o cvfast.o
	ar r visionlib.a visionlib.o geometry.o linesDetection.o image.o cvfast.o structs.o
	ranlib visionlib.a

visionlib.o: visionlib.h
	g++ -c $(CXXFLAGS) visionlib.h -o visionlib.o

geometry.o: geometry.h geometry.cpp
	g++ -c $(CXXFLAGS) geometry.h geometry.cpp

image.o: image.h image.cpp
	g++ -c $(CXXFLAGS) image.h image.cpp

linesDetection.o: linesDetection.h linesDetection.cpp
	g++ -c $(CXXFLAGS) linesDetection.h linesDetection.cpp

cvfast.o: cvfast.cpp
	g++ -c $(CXXFLAGS) cvfast.cpp -o cvfast.o

structs.o: structs.h
	g++ -c $(CXXFLAGS) structs.h -o structs.o

#Clean

clean:
	rm visionlib.a *.o *.gch


