if ENABLE_COMPONENT_MOTIONDETECTION
bin_PROGRAMS = motiondetection #motiondetectiontestmodel
endif

motiondetection_SOURCES = motiondetection.cpp \
			view.cpp view.h \
			model.cpp model.h \
			controller.cpp controller.h
motiondetection_CPPFLAGS =  $(GTKMM_CPPFLAGS) $(OPENCV_CPPFLAGS) \
			$(LIBJDEROBOTICE_CPPFLAGS) \
			-I$(top_srcdir)/src/libs \
			-I$(top_srcdir)/src/interfaces/cpp -DGLADE_DIR=\""$(gladedir)"\"
motiondetection_LDFLAGS = $(GTKMM_LDFLAGS) $(OPENCV_LDFLAGS)
motiondetection_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspaces.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspaces++.la \
			$(top_srcdir)/src/libs/colorspacesice/libcolorspacesice.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

motiondetectiontestmodel_SOURCES = testmodel.cpp model.cpp
motiondetectiontestmodel_CPPFLAGS = $(LIBJDEROBOTICE_CPPFLAGS) -I$(top_srcdir)/src/libs
motiondetectiontestmodel_LDFLAGS = $(OPENCV_LDFLAGS)
motiondetectiontestmodel_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
					$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
					$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
					$(top_srcdir)/src/libs/colorspaces/libcolorspaces.la \
					$(top_srcdir)/src/libs/colorspaces/libcolorspaces++.la

dist_conf_DATA = motiondetection.cfg
dist_glade_DATA = motiondetection.glade
