JDEDIR = /usr/local
INC-DIR = -I../../base -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot -I../../drivers/graphics_xforms
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -O -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

mastergui: masterguigui.o mastergui.o
	$(GCC) -shared -Wl,-soname,mastergui.so $(LIB-DIR) -o mastergui.so mastergui.o masterguigui.o $(LIBS)

mastergui.o: mastergui.c mastergui.h
	$(GCC) $(CFLAGS) -fPIC -c mastergui.c

masterguigui.o:masterguigui.c masterguigui.h
	$(GCC) $(CFLAGS) -fPIC -c masterguigui.c

masterguigui.c: masterguigui.fd
	fdesign -convert masterguigui.fd

clean:
	rm -f *.o mastergui.so masterguigui.c masterguigui.h

install:
	cp mastergui.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h Makefile

dist:
	tar -zcvf mastergui`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile
