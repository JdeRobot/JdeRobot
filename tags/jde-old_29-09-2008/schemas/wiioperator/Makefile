JDEDIR = /usr/local
INC-DIR = -I../../base -I../../drivers/wiimote -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 

wiioperator: wiioperator.o
	$(GCC) -shared -Wl,-soname,wiioperator.so $(LIB-DIR) -o wiioperator.so wiioperator.o  $(LIBS)

wiioperator.o: wiioperator.c
	$(GCC) $(CFLAGS) -fPIC -c wiioperator.c

clean:
	rm -f *.o wiioperator.so

release:
	tar -zcvf wiioperator.tgz *c *h *glade Makefile

