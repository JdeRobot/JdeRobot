PROGRAM = opencvdemo 
PROGRAM_FILE = opencvdemo.cpp viewer.cpp 
FLAGS = -g -Wall -o 
JDEROBOTDIR = /usr/local
CXXFLAGS = -I$(JDEROBOTDIR)/include/jderobot $(shell pkg-config gtkmm-2.4 libglademm-2.4 opencv --cflags)
LDFLAGS = -I$(JDEROBOTDIR)/include/jderobot -I$(JDEROBOTDIR)/lib/jderobot -I/usr/include/gearbox -I/usr/include/gtk-2.0 -I/usr/lib/gearbox  $(shell pkg-config gtkmm-2.4 libglademm-2.4 opencv --cflags --libs)
LDADD = -L$(JDEROBOTDIR)/lib/jderobot -lcolorspacesmm -lJderobotInterfaces -lprogeo -lJderobotUtil -lJderobotIce -DGLADE_DIR=.
CC = g++


opencvdemo: 
	$(CC) $(FLAGS) $(PROGRAM) $(PROGRAM_FILE) $(LDFLAGS) $(LDADD)

