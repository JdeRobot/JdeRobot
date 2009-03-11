##  makefile para al cargadores de Linux
##

INC-DIR = -I. -I../../utils/fuzzylib  -I/usr/local/include 
LIB-DIR = -L. -L../../utils/fuzzylib -L/usr/lib -L -L/usr/local/lib 

CCP= gcc
CFLAGS=  -ansi -pedantic -Wall $(INC-DIR)

all: example

example.o: example.c 
	$(CCP) $(CFLAGS) -c example.c 

example: example.o 
	$(CCP) $(CFLAGS) $(LIB-DIR) -o example example.o -lfuzzy

clean:
	rm -f *.o *.a example
