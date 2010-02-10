JDEDIR = /usr/local
INC-DIR = -I../../base -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I/usr/local/include/player-2.0 
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm -lgazebo

gazebo: gazebo.o 
	$(GCC) -shared -Bsymbolic -Wl,-soname,gazebo.so $(LIB-DIR) -o gazebo.so gazebo.o  $(LIBS)

gazebo.o: gazebo.c 
	$(GCC) $(CFLAGS) -g -fPIC -c gazebo.c

clean:
	rm -f *.o gazebo.so

install:
	cp gazebo.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c makefile
