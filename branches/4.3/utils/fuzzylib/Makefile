##  makefile para al cargadores de Linux
##

INC-DIR = -I. -I../include -I/usr/local/include  
LIB-DIR = -L. -L/usr/lib -L/usr/local/lib

CCP= gcc
CFLAGS=  -ansi -pedantic -Wall $(INC-DIR)

all: lib 

fuzzylib.o: fuzzylib.c  fuzzylib.h
	$(CCP) $(CFLAGS) -c fuzzylib.c -o fuzzylib.o

#genera una libreria estatica
lib: fuzzylib.o fuzzylib.c
	ar r libfuzzy.a fuzzylib.o
	ranlib libfuzzy.a


fuzzylib.debug.o: fuzzylib.c  fuzzylib.h
	$(CCP) $(CFLAGS) -DVERBOSE -c fuzzylib.c -o fuzzylib.debug.o

debug: fuzzylib.debug.o fuzzylib.c
	ar r libfuzzy.a fuzzylib.debug.o
	ranlib libfuzzy.a

clean:
	rm -f *.o *.a
