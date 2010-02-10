JDEDIR = /usr/local
INC-DIR = -I../../base -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include -I/usr/X11R6/include/X11 -I/usr/local/include/player-2.0 -I/usr/include/player-2.0/
LIB-DIR = -L. -L/usr/lib -L -L/usr/local/lib -L/usr/X11R6/lib
GCC = gcc
CFLAGS = -g -pedantic -Bsymbolic -Wall $(INC-DIR)
LIBS = -lm -lplayerc

player: player.o 
	$(GCC) -shared -Bsymbolic -Wl,-soname,player.so $(LIB-DIR) -o player.so player.o  $(LIBS)

player.o: player.c 
	$(GCC) $(CFLAGS) -fPIC -c player.c

clean:
	rm -f *.o player.so

install:
	cp player.so $(JDEDIR)/share/jde-robot/

release:
	tar -zcvf newrelease.tgz *c makefile
