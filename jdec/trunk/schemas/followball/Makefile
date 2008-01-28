JDEDIR = /usr/local
INC-DIR = -I../../base -I../../utils/colorspaces -I. -I/usr/include -I/usr/X11R6/include -I/usr/include/X11 -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot -I../../drivers/graphics_xforms
LIB-DIR = -L../../utils/colorspaces -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -O2 -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms -lcolorspaces 
NAME=followball
VERSION=`cat Changelog | grep ^. | head -1 | cut -d\( -f2 | cut -d\) -f1`

followball: followballgui.o followball.o 
	$(GCC) -shared -Wl,-soname,followball.so $(LIB-DIR) -o followball.so followball.o  followballgui.o $(LIBS)

followballgui.o: followballgui.c followballgui.h
	$(GCC) $(CFLAGS) -fPIC -c followballgui.c

followballgui.c: followballgui.fd
	fdesign -convert followballgui.fd

followball.o: followball.c followball.h
	$(GCC) $(CFLAGS) -fPIC -c followball.c


clean:
	rm -f *.o followball.so followballgui.c followballgui.h
 
install:
	cp followball.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf $(NAME)_$(VERSION).tgz *c *h *fd Makefile Changelog

