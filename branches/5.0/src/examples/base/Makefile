CC=g++
PYTHON=python2.5
CFLAGS=-I. -g -Wall `pkg-config --cflags glib-2.0`
LDFLAGS=-L. `pkg-config --libs glib-2.0`
CSCHEMAS=sA.so

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


all: jde schemas

python: jdepython interfacespython

jdepython: ADDLD = -ljde
jdepython: loader_wrap.c _loader.so schema_wrap.c _schema.so

interfacespython: ADDLD = -linterfaces
interfacespython: random_iface_wrap.c _random_iface.so

jde: libjde.so jdec

jdec: jdec.c
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) -ljde -lreadline -Wl,-export-dynamic $^

libjde.so: loader.c schema.c sfactory.c
	$(CC) -shared $(CFLAGS) $(LDFLAGS) -ldl -Wl,-soname,$@ -o $@ $^


libinterfaces.so: random_iface.c
	$(CC) -shared $(CFLAGS) $(LDFLAGS) -ljde -Wl,-soname,$@ -o $@ $^

schemas: ADDLD = -ljde
schemas: $(CSCHEMAS)

sA.so: sA.c
sAcpp.so: sAcpp.cpp
sB.so: sB.c
sC.so: sC.cpp

clean:
	-rm *.so *.pyc *_wrap.c

.PHONY: all clean python
