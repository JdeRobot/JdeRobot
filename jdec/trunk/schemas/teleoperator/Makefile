JDEDIR = /usr/local
INC-DIR = -I../../base -I../../drivers/graphics_xforms -I../../utils/pioneer -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/include/X11  -I$(JDEDIR)/include/jde-robot
LIB-DIR = -L../../utils/pioneer -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Wall $(INC-DIR)
LIBS = -lm -lX11 -lforms -lformsGL -lGL -lGLU -lpioneer


teleoperator: teleoperatorgui.o pioneeropengl.o teleoperator.o 
	$(GCC) -shared -Wl,-soname,teleoperator.so $(LIB-DIR) -o teleoperator.so teleoperator.o teleoperatorgui.o pioneeropengl.o $(LIBS)

teleoperatorgui.c: teleoperatorgui.fd
	fdesign -code teleoperatorgui.fd

teleoperatorgui.o: teleoperatorgui.c teleoperatorgui.h
	$(GCC) $(CFLAGS) -fPIC -c teleoperatorgui.c

teleoperator.o: teleoperator.c teleoperator.h 
	$(GCC) $(CFLAGS) -fPIC -c teleoperator.c

pioneeropengl.o: pioneeropengl.c pioneeropengl.h
	$(GCC) $(CFLAGS) -fPIC -c pioneeropengl.c

clean:
	rm -f *.o teleoperator.so teleoperatorgui.h teleoperatorgui.c 

install:
	cp teleoperator.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c *h *fd Makefile

