if ENABLE_COMPONENT_RECORDER
bin_PROGRAMS = recorder recorder_client
endif

recorder_SOURCES = recorder.cpp ffmpegRecorder.cpp  ffmpegRecorder.h  GenericRecorder.h GenericRecorder.cpp 

recorder_CXXFLAGS = -I$(top_srcdir)/src/interfaces/cpp $(LIBJDEROBOTICE_CPPFLAGS) 

recorder_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(RECORDER_LIBS)

recorder_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
		 		 $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la 


recorder_client_SOURCES = recorder_client.cpp

recorder_client_CXXFLAGS = -I$(top_srcdir)/src/interfaces/cpp $(LIBJDEROBOTICE_CPPFLAGS)

recorder_client_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(RECORDER_LIBS) -pthread

recorder_client_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
		 		 $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la 


dist_conf_DATA = recorder.cfg
