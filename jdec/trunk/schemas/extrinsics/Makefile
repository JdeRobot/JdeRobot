JDEDIR = /usr/local
INC-DIR = -I../../base -I../../services/graphics_xforms  -I. -I../../libs/progeo -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L.  -L../../libs/progeo/ -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib -L$(JDEDIR)/lib/jde-robot
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms -lprogeo

extrinsics: extrinsicsgui.o extrinsics.o 
	$(GCC) -shared -Wl,-soname,extrinsics.so $(LIB-DIR) -o extrinsics.so extrinsics.o extrinsicsgui.o $(LIBS)

extrinsicsgui.c: extrinsicsgui.fd
	fdesign -code extrinsicsgui.fd

extrinsicsgui.o: extrinsicsgui.c extrinsicsgui.h
	$(GCC) $(CFLAGS) -fPIC -c extrinsicsgui.c

extrinsics.o: extrinsics.c extrinsics.h extrinsicsgui.o
	$(GCC) $(CFLAGS) -fPIC -c extrinsics.c

clean:
	rm -f *.o extrinsics.so extrinsicsgui.h extrinsicsgui.c *.bak *.c~ *.h~

install:
	cp extrinsics.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd *so *.c~ *.h~ *.bak makefile
