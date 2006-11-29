JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../core
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm 

video4linux: video4linux.o 
	$(GCC) -shared -Bsymbolic -Wl,-soname,video4linux.so $(LIB-DIR) -o video4linux.so video4linux.o  $(LIBS)

video4linux.o: video4linux.c 
	$(GCC) $(CFLAGS) -fPIC -c video4linux.c

clean:
	rm -f *.o video4linux.so

install:
	cp video4linux.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c makefile
