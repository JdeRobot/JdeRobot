JDEDIR = /usr/local
EVILIBDIR = ./EVILib/
INC-DIR = -I. -I/usr/include -I/usr/X11R6/include -I/usr/local/include \
          -I/usr/X11R6/include/X11 -I$(JDEDIR)/include/jde-robot \
          -I/usr/include/X11/ -I$(EVILIBDIR) -I../../base
LIB-DIR = -L. -L/usr/lib -L/usr/local/lib -L/usr/X11R6/lib -L /usr/local/lib \
          -L $(EVILIBDIR)
GCC = gcc
GPP = g++
CFLAGS = -g -ggdb -Wall -fPIC -ffast-math -O6 -D_SMP_ -DMODULE -D_KERNEL_ \
         -fexpensive-optimizations
LIBS = -lm -lX11 -lforms -lXext -lpthread -lstdc++
WD = $(PWD)

evi.so: evi.o camera.o $(EVILIBDIR)/EVILib.a
	$(GPP) -g $(CFLAGS) -shared -Wl,-soname,evi.so $(LIB-DIR) $(LIBS) -o evi.so evi.o camera.o $(EVILIBDIR)/EVILib.a

evi.o: evi.c
	$(GPP) $(CFLAGS) $(INC-DIR) -fPIC -c $<

camera.o: camera.cpp
	$(GPP) $(CFLAGS) -fPIC $(INC-DIR) -o $@ -c $<

$(EVILIBDIR)/EVILib.a:
	cd $(EVILIBDIR); make;

install:
	cp evi.so $(JDEDIR)/share/jde-robot/

clean:
	rm -f *.o evi.so; cd $(EVILIBDIR); make rclean;

release:
	tar -zcvf newrelease.tgz *c *cpp *h $(EVILIBDIR) Makefile
