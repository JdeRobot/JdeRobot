LIBS = -I/usr/include/gtk-2.0/gdk/ -I/usr/include/gtk-2.0/gtk/ -I/usr/include/gtk-2.0/gdk-pixbuf/ -I/usr/include/libgnomecanvas-2.0/libgnomecanvas/ -I/usr/include/libxml2/libxml/
SRC = src
LIB_DIR = `pkg-config --cflags --libs gtk+-2.0 libgnomecanvas-2.0 libglade-2.0 gtksourceview-2.0 gtk+-unix-print-2.0`
GPP = g++
CFLAGS = -g -Wall 
SOURCES = int2string.o ./recta/recta.o nodo.o transicion.o ventanas.o xml.o imprimir.o

#g++ -g -Wall -c transicion.cpp `pkg-config --cflags --libs gtk+-2.0 libgnomecanvas-2.0`
#g++ -g -Wall interfaz.cpp -o interfaz ./src/recta/recta.o ./src/nodo.o ./src/transicion.o ./src/ventanas.o `pkg-config --cflags --libs gtk+-2.0 libgnomecanvas-2.0 libglade-2.0 gtksourceview-1.0`

ALL: visualHFSM

generate.o: generate.cpp reemplazar.o
	$(GPP) $(CFLAGS) $(LIB_DIR) $(LIBS) generate.cpp -o generate reemplazar.o 
	
reemplazar.o: reemplazar.cpp
	$(GPP) $(CFLAGS) $(LIB_DIR) $(LIBS) -c reemplazar.cpp -o reemplazar.o 

visualHFSM: generate.o $(SOURCES) interfaz.o 
	$(GPP) $(CFLAGS) $(LIB_DIR) $(LIBS) -o visualHFSM interfaz.o $(SOURCES)

interfaz.o: interfaz.cpp
	$(GPP) $(CFLAGS) -c interfaz.cpp  $(LIB_DIR)

imprimir.o: imprimir.cpp imprimir.h
	$(GPP) $(CFLAGS) $(LIB_DIR) -c imprimir.cpp 

xml.o: xml.cpp xml.h transicion.o nodo.o
	$(GPP) $(CFLAGS) $(LIB_DIR) -c xml.cpp 

ventanas.o: ventanas.cpp transicion.o nodo.o
	$(GPP) $(CFLAGS) $(LIB_DIR) -c ventanas.cpp

nodo.o: nodo.cpp ./recta/recta.o
	$(GPP) $(CFLAGS) $(LIB_DIR) -c nodo.cpp

transicion.o: transicion.cpp ./recta/recta.o
	$(GPP) $(CFLAGS) $(LIB_DIR) -c transicion.cpp

./recta/recta.o: ./recta/recta.c ./recta/recta.h
	$(GPP) $(CFLAGS) $(LIB_DIR) -c recta/recta.c -o ./recta/recta.o

int2string.o: int2string.cpp int2string.h
	$(GPP) $(CFLAGS) $(LIB_DIR) -c int2string.cpp 

clean:
	rm -f *.o *~ 
 
