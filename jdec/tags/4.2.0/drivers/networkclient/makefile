INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I/usr/local/include/jde-robot
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib 
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm 

networkclient: networkclient.o
	$(GCC) -shared -Bsymbolic -Wl,-soname,networkclient.so.1 $(LIB-DIR) -o networkclient.so.1.0 networkclient.o $(LIBS)

networkclient.o: networkclient.c
	$(GCC) $(CFLAGS) -fPIC -c networkclient.c

clean:
	rm -f *.o networkclient.so.1.0

release:
	tar -zcvf newrelease.tgz *c makefile
