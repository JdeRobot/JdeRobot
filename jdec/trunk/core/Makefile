INC-DIR = -I. -I../include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
#-ansi
#LIBS = -lpthread -lm -lforms -lX11 -ldc1394_control -lraw1394
LIBS = -lpthread -lm -lforms -lX11 -ldl
OBJS = jdegui.o mastergui.o sensorsmotorsgui.o
INSTALLDIR=/usr/local
CFGDIR="$(INSTALLDIR)/jde-robot"

ALL: jde

# Infraestructura jde
jde: jde.o $(OBJS)
	$(GCC) $(CFLAGS) -Wl,-export-dynamic $(LIB-DIR) -o jde jde.o $(OBJS) $(LIBS)

jde.o: jde.c
	$(GCC) $(CFLAGS) -D CONFIGDIR='$(CFGDIR)' -c jde.c	

jdegui.o: jdegui.c jdegui.h
	$(GCC) $(CFLAGS) -c jdegui.c

mastergui.o: mastergui.c mastergui.h
	$(GCC) $(CFLAGS) -c mastergui.c

sensorsmotorsgui.o: sensorsmotorsgui.c sensorsmotorsgui.h
	$(GCC) $(CFLAGS) -c sensorsmotorsgui.c

clean:
	rm -f *.o *.a *so.1.0

install:
	cp jde $(INSTALLDIR)/bin/
	cp jde.h $(INSTALLDIR)/include/jde-robot/

release:
	tar -zcvf newrelease.tgz *h *c *fd *.conf README makefile