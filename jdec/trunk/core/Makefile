INC-DIR = -I. -I../include -I/usr/local/include
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lpthread -lm -ldl

INSTALLDIR=/usr/local
CFGDIR="$(INSTALLDIR)/jde-robot"

ALL: jde

# Infraestructura jde
jde: jde.o
	$(GCC) $(CFLAGS) -Wl,-export-dynamic $(LIB-DIR) -o jde jde.o $(OBJS) $(LIBS)

jde.o: jde.c
	$(GCC) $(CFLAGS) -D CONFIGDIR='$(CFGDIR)' -c jde.c

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
