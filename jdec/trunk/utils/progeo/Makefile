##  makefile para al cargadores de Linux
##


CCP=         gcc
CFLAGS=      -O2 

all:lib

progeo.o: progeo.c  progeo.h
	$(CCP) $(CFLAGS) -c progeo.c -o progeo.o

#genera una libreria estatica
lib: progeo.o
	ar r libprogeo.a progeo.o
	ranlib libprogeo.a

release:
	tar -zcvf newreleaste.tgz progeo.c progeo.h Makefile

clean:
	rm -f *.o libprogeo.a







