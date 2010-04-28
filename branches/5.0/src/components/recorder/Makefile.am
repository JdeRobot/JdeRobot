if ENABLE_COMPONENT_RECORDER
bin_PROGRAMS = recorder recorder_client
endif

recorder_SOURCES = recorder.cpp deviceRecorder.cpp deviceRecorder.h  GenericRecorder.h GenericRecorder.cpp imageRecorder.cpp imageRecorder.h

recorder_CPPFLAGS = -I$(top_srcdir)/src/interfaces/cpp $(LIBJDEROBOTICE_CPPFLAGS) $(LIBCOLORSPACESMM_LDFLAGS)

recorder_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(RECORDER_LIBS) $(LIBICEBOX) $(LIBICESTORM) $(LIBICESTORMSERVICE) $(LIBCOLORSPACESMM_LDFLAGS)

recorder_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
		 		 $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
				 $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la


recorder_client_SOURCES = recorder_client.cpp

recorder_client_CPPFLAGS = -I$(top_srcdir)/src/interfaces/cpp $(LIBJDEROBOTICE_CPPFLAGS)

recorder_client_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(RECORDER_LIBS) -pthread

recorder_client_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
		 		 $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la 


dist_conf_DATA = recorder.cfg
