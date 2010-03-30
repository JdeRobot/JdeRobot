if ENABLE_COMPONENT_RECORDER
bin_PROGRAMS = recorder
endif

recorder_SOURCES = recorder.cpp 

recorder_CXXFLAGS = -I$(top_srcdir)/src/interfaces/cpp

recorder_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(RECORDER_LIBS)

recorder_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = recorder.cfg