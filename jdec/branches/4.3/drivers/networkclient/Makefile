JDEDIR = /usr/local
INC-DIR = -I. -I../../base -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I/usr/local/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm 

networkclient: networkclient.o
	$(GCC) -shared -Bsymbolic -Wl,-soname,networkclient.so $(LIB-DIR) -o networkclient.so networkclient.o $(LIBS)

networkclient.o: networkclient.c
	$(GCC) $(CFLAGS) -fPIC -c networkclient.c

clean:
	rm -f *.o networkclient.so

install:
	cp networkclient.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c makefile
