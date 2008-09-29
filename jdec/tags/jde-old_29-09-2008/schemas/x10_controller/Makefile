JDEDIR = /usr/local
INC-DIRA = `pkg-config --cflags libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
LIB-DIRA = `pkg-config --libs libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
INC-DIR = -I../../base -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot -I../../drivers/x10/ -I../../drivers/graphics_gtk $(INC-DIRA)
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib $(LIB-DIRA)
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 

x10_controller: x10_controller.o
	$(GCC) -shared -Wl,-soname,x10_controller.so $(LIB-DIR) -o x10_controller.so x10_controller.o  $(LIBS)

x10_controller.o: x10_controller.c
	$(GCC) $(CFLAGS) -fPIC -c x10_controller.c

clean:
	rm -f *.o x10_controller.so

release:
	tar -zcvf x10_controller.tgz *c *h *glade Makefile

