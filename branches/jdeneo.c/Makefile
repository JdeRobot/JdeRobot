INC-DIR = `pkg-config --cflags libglade-2.0 gthread-2.0 libgnomecanvas-2.0 gtkglext-1.0 gdkglext-1.0`
LIB-DIR = `pkg-config --libs libglade-2.0 gthread-2.0 libgnomecanvas-2.0 gtkglext-1.0 gdkglext-1.0`
GCC = gcc
CFLAGS = -g -Wall -rdynamic $(INC-DIR)
LIBS = -ldc1394_control -lraw1394
SCHEMAS = schemas.o preferences.o gui.o trackball.o pioneer.o visualization3d.o cellrendererspin.o


BIN=$(DESTDIR)/usr/bin
ETC=$(DESTDIR)/etc

ALL: jde

jde: $(SCHEMAS) jde.o
	$(GCC) $(CFLAGS) $(LIB-DIR) -o jdeneo jde.o $(SCHEMAS) $(LIBS)

jde.o: jde.c
	$(GCC) $(CFLAGS) -D RESOURCEDIR='"./gui"' -D JDEFIREWIRE -D JDEVIDEO4LINUX -c jde.c	

preferences.o: preferences.c preferences.h
	$(GCC) $(CFLAGS) -c preferences.c

schemas.o: schemas.c schemas.h
	$(GCC) $(CFLAGS) -c schemas.c

gui.o: gui.c gui.h
	$(GCC) $(CFLAGS) -D RESOURCEDIR='"./gui"' -c gui.c

trackball.o: trackball.c trackball.h
	$(GCC) $(CFLAGS) -c trackball.c

visualization3d.o: visualization3d.c visualization3d.h
	$(GCC) $(CFLAGS) -c visualization3d.c

pioneer.o: pioneer.c pioneer.h
	$(GCC) $(CFLAGS) -c pioneer.c

cellrendererspin.o: cellrendererspin.c cellrendererspin.h
	$(GCC) $(CFLAGS) -c cellrendererspin.c

clean:
	rm -f *.o *~ jdeneo *tgz
release:
	tar -zcvf newrelease.tgz *h *c Makefile gui/jde* gui/pixmaps/*png

install: jde
	install -d $(BIN)
	install -m755 jde $(BIN)
	install -d $(ETC)
