
GCC = gcc
CFLAGS = -g -O2 -Wall
LIBS = -lm 

NAME="libpioneer"
VERSION=`cat Changelog | grep ^. | head -1 | cut -d\( -f2 | cut -d\) -f1`

all: lib

pioneer.c: pioneer.h
	touch pioneer.c

pioneer.o : pioneer.c
	$(GCC) $(CFLAGS) -fPIC -c $<

#genera una libreria estatica
lib: pioneer.o
	ar r $(NAME).a $<
	ranlib $(NAME).a

install: lib
	cp $(NAME).a /usr/lib
	cp pioneer.h /usr/include

uninstall: all
	rm -f /usr/lib/$(NAME).a
	rm -f /usr/include/pioneer.h

newrelease:
	tar -zcvf $(NAME)_$(VERSION).tgz *c *.h Makefile Changelog

clean:
	rm -f *.o *.a 
