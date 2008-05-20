JDEDIR = /usr/local
INC-DIRA = `pkg-config --cflags libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
LIB-DIRA = `pkg-config --libs libglade-2.0 gtkglext-1.0 gdkglext-1.0 gdkglext-1.0 gthread-2.0`
INC-DIR = -I../../base -I../../drivers/graphics_gtk -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot $(INC-DIRA)
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib $(LIB-DIRA)
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm 

opengl_viewer: opengl_viewer.o obj_loader.o
	$(GCC) -shared -Wl,-soname,opengl_viewer.so $(LIB-DIR) -o opengl_viewer.so opengl_viewer.o obj_loader.o $(LIBS)

opengl_viewer.o: opengl_viewer.c
	$(GCC) $(CFLAGS) -fPIC -c opengl_viewer.c

obj_loader.o: obj_loader.c obj_loader.h
	$(GCC) $(CFLAGS) -fPIC -c obj_loader.c

clean:
	rm -f *.o opengl_viewer.so

release:
	tar -zcvf opengl_viewer`date +%Y-%m-%d_%H-%M`.tgz *c *h *glade Makefile

