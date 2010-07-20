if ENABLE_COMPONENT_SURVEILLANCE
bin_PROGRAMS = surveillance
endif


surveillance_SOURCES = surveillance.cpp
surveillance_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS)
surveillance_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(RECORDER_LIBS) -pthread
surveillance_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
			$(top_srcdir)/src/libs/jderobotice/libJderobotIce.la 

dist_conf_DATA = surveillance.cfg
