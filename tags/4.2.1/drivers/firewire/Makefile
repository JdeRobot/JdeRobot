JDEDIR = /usr/local
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I../../base
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm -ldc1394_control -lraw1394

firewire: firewire.o 
	$(GCC) -shared -Bsymbolic -Wl,-soname,firewire.so $(LIB-DIR) -o firewire.so firewire.o  $(LIBS)

firewire.o: firewire.c 
	$(GCC) $(CFLAGS) -fPIC -c firewire.c

clean:
	rm -f *.o firewire.so

install:
	cp firewire.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c makefile

