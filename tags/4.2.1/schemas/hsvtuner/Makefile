JDEDIR = /usr/local
INC-DIR = -I../../base -I../../utils/colorspaces -I. -I/usr/include -I/usr/X11R6/include -I/usr/include/X11 -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot -I../../drivers/graphics_xforms 
LIB-DIR = -L. -L../../utils/colorspaces -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib  
GCC = gcc
CFLAGS = -g -O2 -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms -lcolorspaces

hsvtuner: hsvtuner.o hsvtunergui.o
	$(GCC) -shared -Wl,-soname,hsvtuner.so $(LIB-DIR) -o hsvtuner.so hsvtuner.o  hsvtunergui.o $(LIBS)

hsvtunergui.o: hsvtunergui.c hsvtunergui.h
	$(GCC) $(CFLAGS) -fPIC -c hsvtunergui.c

hsvtunergui.c: hsvtunergui.fd
	fdesign -convert hsvtunergui.fd

hsvtuner.o: hsvtuner.c hsvtuner.h hsvtunergui.o
	$(GCC) $(CFLAGS) -fPIC -c hsvtuner.c

clean:
	rm -f *.o hsvtuner.so hsvtunergui.c hsvtunergui.h
 
install:
	cp hsvtuner.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd Makefile

