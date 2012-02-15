JDEROBOTDIR = /usr/local
CXXFLAGS = -I$(JDEROBOTDIR)/include/jderobot `pkg-config --cflags opencv`

LDFLAGS =  `pkg-config --libs opencv`

     
LDADD = $(JDEROBOTDIR)/lib/jderobot/libJderobotInterfaces.la \
	     $(JDEROBOTDIR)/lib/jderobot/libJderobotUtil.la \
        $(JDEROBOTDIR)/lib/jderobot/libcolorspacesmm.la 

introrob: recorder.o
	libtool --mode=link g++ -g -O -o recorder recorder.o  $(LDADD) $(LDFLAGS)

clean:
	rm -f *.o recorder


