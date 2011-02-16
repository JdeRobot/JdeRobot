if ENABLE_COMPONENT_RECORDINGSERVER
bin_PROGRAMS = recordingserver
endif

recordingserver_SOURCES = recordingserver.cpp \
                          libRecordingLog/RecordingLog.cpp libRecordingLog/RecordingLog.h
recordingserver_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS)
recordingserver_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(RECORDINGSERVER_LIBS)
recordingserver_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = recordingserver.cfg
