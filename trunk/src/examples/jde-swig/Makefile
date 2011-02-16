CC=g++
PYTHON=python2.5
CFLAGS=-I. -I/usr/include/$(PYTHON) -g -Wall
LDFLAGS=-L.
CSCHEMAS=sA.so sB.so sC.so sAcpp.so

#genera wrapper para python
%_wrap.c: %.i
	swig -python $<

#genera libreria dinamica con wrapper python
_%.so: %_wrap.c
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $(ADDLD) -Wl,-soname,$@ -o $@ $^

#genera libreria dinamica con esquema C
%.so: %.c
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $(ADDLD) -Wl,-soname,$@ -o $@ $^

#genera libreria dinamica con esquema C++
%.so: %.cpp
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $(ADDLD) -Wl,-soname,$@ -o $@ $^


all: jde python schemas

python: jdepython interfacespython

jdepython: ADDLD = -ljde
jdepython: loader_wrap.c _loader.so schema_wrap.c _schema.so

interfacespython: ADDLD = -linterfaces
interfacespython: random_iface_wrap.c _random_iface.so

jde: libjde.so libinterfaces.so

libjde.so: ADDLD = -ldl
libjde.so: loader.c schema.c
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $(ADDLD) -Wl,-soname,$@ -o $@ $^

libinterfaces.so: ADDLD = -ljde
libinterfaces.so: random_iface.c
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $(ADDLD) -Wl,-soname,$@ -o $@ $^

schemas: ADDLD = -ljde -linterfaces
schemas: $(CSCHEMAS)

sA.so: sA.c
sAcpp.so: sAcpp.cpp
sB.so: sB.c
sC.so: sC.cpp

clean:
	-rm *.so *.pyc *_wrap.c

.PHONY: all clean python

#jdec: ADDLD = -ljde -ldl -l$(PYTHON)
#jdec: main.c
#	$(CC) -o $@ $(CFLAGS) -Wl,-export-dynamic $(ADDLD) $^
