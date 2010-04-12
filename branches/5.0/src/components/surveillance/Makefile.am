if ENABLE_COMPONENT_SURVEILLANCE
bin_PROGRAMS = surveillance
endif


surveillance_SOURCES = surveillance.cpp

surveillance_CXXFLAGS = -I$(top_srcdir)/src/interfaces/cpp $(LIBJDEROBOTICE_CPPFLAGS)

surveillance_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(RECORDER_LIBS) -pthread

surveillance_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la \
 		     $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la 


dist_conf_DATA = surveillance.cfg
