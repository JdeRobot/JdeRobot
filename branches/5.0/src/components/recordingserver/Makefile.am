if ENABLE_COMPONENT_RECORDINGSERVER
bin_PROGRAMS = recordingserver
endif

recordingserver_SOURCES = recordingserver.cpp \
                          libRecordingLog/RecordingLog.cpp libRecordingLog/RecordingLog.h

recordingserver_CXXFLAGS = -I$(top_srcdir)/src/interfaces/cpp

recordingserver_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(RECORDINGSERVER_LIBS)

recordingserver_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = recordingserver.cfg
