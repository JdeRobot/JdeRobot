CXXFLAGS = -I $ -I. -I/usr/include/gearbox/ `pkg-config --cflags opencv` `pkg-config --cflags gtkmm-2.4 libglademm-2.4 gthread-2.0` -I/usr/include/gtkglextmm-1.2/ -I/usr/lib/gtkglextmm-1.2/include/ -I/usr/lib/gtkglext-1.0/include/ -I/usr/include/gtkglext-1.0/ -I $(JDEROBOTDIR)/include/jderobot `pkg-config --cflags gtk+-2.0 gtkgl-2.0`
LDFLAGS = -lIce -lIceUtil -lIceGrid -lIceBox -lIceStorm -lIceStormService -L/usr/lib/gearbox -lGbxUtilAcfr -lGbxIceUtilAcfr -pthread -lglademm-2.4 -lgtkmm-2.4 -lglade-2.0 -latkmm-1.6 -lgdkmm-2.4 -lgiomm-2.4 -lpangomm-1.4 -lglibmm-2.4 -lcairomm-1.0 -lsigc-2.0 -lgtk-x11-2.0 -lxml2 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0   -lcv -lhighgui -lcvaux -lml -lcxcore   -Wl,--export-dynamic -pthread -lgtkglextmm-x11-1.2 -lgdkglextmm-x11-1.2 -lgtkglext-x11-1.0 -lgtkmm-2.4 -lgdkglext-x11-1.0 -lGLU -lGL -lXmu -lXt -lSM -lICE -lpangox-1.0 -lX11 -latkmm-1.6 -lgdkmm-2.4 -lgiomm-2.4 -lpangomm-1.4 -lgtk-x11-2.0 -lglibmm-2.4 -lcairomm-1.0 -lsigc-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0   -lGLU -lGL   -lglut 
LDADD = $(JDEROBOTDIR)/lib/jderobot/libJderobotIce.la \
        $(JDEROBOTDIR)/lib/jderobot/libJderobotUtil.la \
        $(JDEROBOTDIR)/lib/jderobot/libcolorspacesmm.la \
        $(JDEROBOTDIR)/lib/jderobot/libJderobotInterfaces.la \
        $(JDEROBOTDIR)/lib/jderobot/libprogeo.la

kinectViewer: kinectViewer.o kinectViewergui.o drawarea.o myprogeo.o util3d.o controllers/ptmotors-controller.o gui-modules/ptmotorsGui.o controllers/leds-controller.o gui-modules/ledsGui.o
	libtool --mode=link g++ -g -O -o kinectViewer kinectViewer.o kinectViewergui.o drawarea.o myprogeo.o util3d.o controllers/ptmotors-controller.o gui-modules/ptmotorsGui.o controllers/leds-controller.o gui-modules/ledsGui.o $(LDADD) $(LDFLAGS)

clean:
	rm -rf *.o kinectViewer *.gch kinectViewer
