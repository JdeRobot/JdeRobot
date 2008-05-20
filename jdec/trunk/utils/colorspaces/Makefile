
GCC = gcc
CFLAGS = -g -O2 -Wall
LIBS = -lm 

NAME="libcolorspaces"
VERSION=`cat Changelog | grep ^. | head -1 | cut -d\( -f2 | cut -d\) -f1`

all: lib

rgb2hsv.o : rgb2hsv.c colorspaces.h
	$(GCC) $(CFLAGS) -fPIC -c rgb2hsv.c

#genera una libreria estatica
lib: rgb2hsv.o
	ar r libcolorspaces.a rgb2hsv.o
	ranlib libcolorspaces.a

install: lib
	cp libcolorspaces.a /usr/lib
	cp colorspaces.h /usr/include

uninstall:
	rm -f /usr/lib/libcolor-spaces.a
	rm -f /usr/include/colorspaces.h

newrelease:
	tar -zcvf $(NAME)_$(VERSION).tgz *c *.h Makefile Changelog
clean:
	rm -f *.o *.a 
