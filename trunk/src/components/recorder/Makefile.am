if ENABLE_COMPONENT_RECORDER
bin_PROGRAMS = recorder recorder_client
endif

recorder_SOURCES = recorder.cpp deviceRecorder.cpp deviceRecorder.h \
			GenericRecorder.h GenericRecorder.cpp imageRecorder.cpp imageRecorder.h
recorder_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(COLORSPACESMM_LDFLAGS)
recorder_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(RECORDER_LIBS) $(COLORSPACESMM_LDFLAGS)
recorder_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
		 		 $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
				 $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la

recorder_client_SOURCES = recorder_client.cpp
recorder_client_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS)
recorder_client_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(RECORDER_LIBS) -pthread
recorder_client_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
		 		 $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la 


dist_conf_DATA = recorder.cfg
