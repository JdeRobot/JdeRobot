bin_PROGRAMS = alarmgenerator

alarmgenerator_SOURCES = alarmgenerator.cpp
alarmgenerator_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS)
alarmgenerator_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(ALARMGENERATOR_LIBS)
alarmgenerator_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = alarmgenerator.cfg
