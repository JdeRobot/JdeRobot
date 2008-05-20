JDEDIR = /usr/local
INC-DIRA = `pkg-config --cflags libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
LIB-DIRA = `pkg-config --libs libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
INC-DIR = -I../../base -I../../drivers/graphics_gtk -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot $(INC-DIRA)
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib $(LIB-DIRA)
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

esquema_prueba: esquema_prueba.o
	$(GCC) -shared -Wl,-soname,esquema_prueba.so $(LIB-DIR) -o esquema_prueba.so esquema_prueba.o  $(LIBS)

esquema_prueba.o: esquema_prueba.c
	$(GCC) $(CFLAGS) -fPIC -c esquema_prueba.c

clean:
	rm -f *.o esquema_prueba.so

release:
	tar -zcvf newrelease.tgz *c *h *fd makefile

