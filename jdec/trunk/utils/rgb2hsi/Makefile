
GCC = gcc
CFLAGS = -g -O2 -Wall
LIBS = -lm 

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
	tar -zcvf newrelease.tgz *c *.h Makefile Changelog

clean:
	rm -f *.o *.a 
