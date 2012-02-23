JDEROBOTDIR = /usr/local
CXXFLAGS = -Wall -g -I. -I$(JDEROBOTDIR)/include/jderobot `pkg-config --cflags opencv` `pkg-config --cflags gtkmm-2.4 libglademm-2.4 gthread-2.0` `pkg-config --cflags gtkglextmm-1.2`
LDFLAGS = `pkg-config --libs gtkmm-2.4 libglademm-2.4 gthread-2.0` `pkg-config --libs opencv` -lgsl -lgslcblas `pkg-config --libs gtkglextmm-1.2`
LDADD = $(JDEROBOTDIR)/lib/jderobot/libJderobotIce.so \
	$(JDEROBOTDIR)/lib/jderobot/libJderobotUtil.so \
	$(JDEROBOTDIR)/lib/jderobot/libJderobotInterfaces.so \
	$(JDEROBOTDIR)/lib/jderobot/libprogeo.so

all: visionlibrary.o geometry.o image.o linesDetection.o cvfast.o
	ar r visionlibrary.a visionlibrary.o geometry.o image.o image.o cvfast.o
	ranlib visionlibrary.a

visionlibrary.o: visionlibrary.h
	g++ -c $(CXXFLAGS) visionlibrary.h -o visionlibrary.o

geometry.o: geometry.cpp
	g++ -c $(CXXFLAGS) geometry.cpp -o geometry.o

image.o: image.cpp
	g++ -c $(CXXFLAGS) image.cpp -o image.o

linesDetection.o: linesDetection.cpp
	g++ -c $(CXXFLAGS) linesDetection.cpp -o linesDetection.o

cvfast.o: cvfast.cpp
	g++ -c $(CXXFLAGS) cvfast.cpp -o cvfast.o

#Clean

clean:
	rm visionlibrary.a *.o *.gch


