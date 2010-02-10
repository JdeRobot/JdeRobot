JDEDIR = /usr/local
INC-DIR = -I../../core -I. -I/usr/include -I/usr/X11R6/include -I/usr/include/X11 -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot 
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

hsituner: hsituner.o hsitunergui.o
	$(GCC) -shared -Wl,-soname,hsituner.so $(LIB-DIR) -o hsituner.so hsituner.o  hsitunergui.o $(LIBS)

hsitunergui.o: hsitunergui.c hsitunergui.h
	$(GCC) $(CFLAGS) -fPIC -c hsitunergui.c

hsitunergui.c: hsitunergui.fd
	fdesign -convert hsitunergui.fd

hsituner.o: hsituner.c hsituner.h hsitunergui.o
	$(GCC) $(CFLAGS) -fPIC -c hsituner.c

clean:
	rm -f *.o hsituner.so
 
install:
	cp hsituner.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

