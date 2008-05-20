JDEDIR = /usr/local
INC-DIRA = `pkg-config --cflags libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
LIB-DIRA = `pkg-config --libs libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
INC-DIR = -I../../base -I../../drivers/graphics_gtk -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot $(INC-DIRA)
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib $(LIB-DIRA)
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 

image_viewer: image_viewer.o
	$(GCC) -shared -Wl,-soname,image_viewer.so $(LIB-DIR) -o image_viewer.so image_viewer.o  $(LIBS)

image_viewer.o: image_viewer.c
	$(GCC) $(CFLAGS) -fPIC -c image_viewer.c

clean:
	rm -f *.o image_viewer.so

release:
	tar -zcvf image_viewer.tgz *c *h *glade Makefile

