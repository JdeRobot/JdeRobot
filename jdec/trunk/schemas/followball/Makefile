JDEDIR = /usr/local
INC-DIR = -I../../core -I. -I/usr/include -I/usr/X11R6/include -I/usr/include/X11 -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot 
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -O2 -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

followball: followballgui.o followball.o libRGB2HSI.o
	$(GCC) -shared -Wl,-soname,followball.so $(LIB-DIR) -o followball.so followball.o  followballgui.o libRGB2HSI.o $(LIBS)

followballgui.o: followballgui.c followballgui.h
	$(GCC) $(CFLAGS) -fPIC -c followballgui.c

followballgui.c: followballgui.fd
	fdesign -convert followballgui.fd

followball.o: followball.c followball.h
	$(GCC) $(CFLAGS) -fPIC -c followball.c

libRGB2HSI.o: libRGB2HSI.c libRGB2HSI.h
	$(GCC) $(CFLAGS) -fPIC -c libRGB2HSI.c


clean:
	rm -f *.o followball.so
 
install:
	cp followball.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile Changelog

