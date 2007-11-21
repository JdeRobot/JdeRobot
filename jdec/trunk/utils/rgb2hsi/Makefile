
GCC = gcc
CFLAGS = -g -O2 -Wall
LIBS = -lm 

NAME="libRGB2HSI"
VERSION=`cat Changelog | grep ^. | head -1 | cut -d\( -f2 | cut -d\) -f1`

all: lib

rgb2hsi.o : rgb2hsi.c rgb2hsi.h
	$(GCC) $(CFLAGS) -fPIC -c rgb2hsi.c

#genera una libreria estatica
lib: rgb2hsi.o
	ar r libRGB2HSI.a rgb2hsi.o
	ranlib libRGB2HSI.a

install: lib
	cp libRGB2HSI.a /usr/lib
	cp rgb2hsi.h /usr/include
	
uninstall:
	rm -f /usr/lib/libRGB2HSI.a
	rm -f /usr/include/rgb2hsi.h

newrelease:
	tar -zcvf $(NAME)_$(VERSION).tgz *c *.h Makefile Changelog
clean:
	rm -f *.o *.a 
