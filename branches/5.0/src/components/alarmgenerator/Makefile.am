bin_PROGRAMS = alarmgenerator

alarmgenerator_SOURCES = alarmgenerator.cpp

alarmgenerator_CXXFLAGS = -I$(top_srcdir)/src/interfaces/cpp

alarmgenerator_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(ALARMGENERATOR_LIBS)

alarmgenerator_LDADD = $(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = alarmgenerator.cfg
