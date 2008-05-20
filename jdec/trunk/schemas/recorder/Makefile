JDEDIR = /usr/local
INC-DIR = -I../../base -I../../drivers/graphics_xforms -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11 -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -O -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms 

recorder: recordergui.o recorder.o 
	$(GCC) -shared -Wl,-soname,recorder.so $(LIB-DIR) -o recorder.so recorder.o recordergui.o $(LIBS)

recorder.o: recorder.c recorder.h
	$(GCC) $(CFLAGS) -fPIC -c recorder.c

recordergui.o:recordergui.c recordergui.h
	$(GCC) $(CFLAGS) -fPIC -c recordergui.c

recordergui.c: recordergui.fd
	fdesign -convert recordergui.fd

clean:
	rm -f *.o recorder.so recordergui.c recordergui.h

install:
	cp recorder.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h Makefile

dist:
	tar -zcvf recorder`date +%Y-%m-%d_%H-%M`.tgz *c *h *fd Makefile
