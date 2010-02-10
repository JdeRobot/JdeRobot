INC-DIR = -I. -I../include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
#-ansi
#LIBS = -lpthread -lm -lforms -lX11 -ldc1394_control -lraw1394
LIBS = -lpthread -lm -lforms -lX11 -ldl -lformsGL -lGL -lGLU
OBJS = jdegui.o mastergui.o sensorsmotorsgui.o pioneer.o 
INSTALLDIR=/usr/local
CFGDIR="$(INSTALLDIR)/jde-robot"

ALL: jde

# Infraestructura jde
jde: jde.o $(OBJS)
	$(GCC) $(CFLAGS) -Wl,-export-dynamic $(LIB-DIR) -o jde jde.o $(OBJS) $(LIBS)

jde.o: jde.c
	$(GCC) $(CFLAGS) -D CONFIGDIR='$(CFGDIR)' -c jde.c	

jdegui.o: jdegui.c jdegui.h mastergui.o sensorsmotorsgui.o
	$(GCC) $(CFLAGS) -c jdegui.c

mastergui.o: mastergui.c mastergui.h
	$(GCC) $(CFLAGS) -c mastergui.c

mastergui.c: mastergui.fd
	fdesign -convert mastergui.fd

sensorsmotorsgui.o: sensorsmotorsgui.c sensorsmotorsgui.h
	$(GCC) $(CFLAGS) -c sensorsmotorsgui.c

sensorsmotorsgui.c: sensorsmotorsgui.fd
	fdesign -convert sensorsmotorsgui.fd

pioneer.o: pioneer.c pioneer.h
	$(GCC) $(CFLAGS) -fPIC -c pioneer.c

clean:
	-rm -f *.o *.a *so *~

install:
	cp jde $(INSTALLDIR)/bin/
	cp jde.h $(INSTALLDIR)/include/jde-robot/
	cp jdegui.h $(INSTALLDIR)/include/jde-robot/	
	cp jde.conf $(INSTALLDIR)/share/jde-robot/	
	cp ponpath $(INSTALLDIR)/share/jde-robot/		

release:
	tar -zcvf newrelease.tgz *h *c *fd *.conf Makefile
