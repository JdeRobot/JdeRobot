if ENABLE_COMPONENT_MYCOMPONENT
bin_PROGRAMS = mycomponent
endif

mycomponent_SOURCES = 	mycomponent.cpp \
			control.cpp \
			control.h \
			camera.cpp \
			camera.h \
			API.cpp \
			API.h \
			gui.h \
			gui.cpp \
			canvasTeleoperateControl.h \
			canvasTeleoperateControl.cpp \
			canvasTeleoperateCameras.h \
			canvasTeleoperateCameras.cpp \
			canvasLaser.h \
			canvasLaser.cpp
			
						
mycomponent_CPPFLAGS =  $(GTKMM_CPPFLAGS) $(OPENCV_CPPFLAGS) $(COLORSPACESMM_CPPFLAGS) \
			$(GSL_CPPFLAGS) $(OPENGL_CPPFLAGS) $(GTKGLEXTMM_CPPFLAGS) $(JDEROBOTICE_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
mycomponent_LDFLAGS = $(JDEROBOTICE_LDFLAGS) $(GTKMM_LDFLAGS) $(OPENCV_LDFLAGS) $(COLORSPACESMM_LDFLAGS) $(GTKGLEXTMM_LDFLAGS) $(OPENGL_LDFLAGS) $(GSL_LDFLAGS)
mycomponent_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspaces.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = mycomponent.cfg
dist_glade_DATA = mycomponent.glade
