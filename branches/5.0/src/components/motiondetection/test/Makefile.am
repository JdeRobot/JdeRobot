if ENABLE_COMPONENT_MOTIONDETECTION
bin_PROGRAMS = motiondetectiontestmodel motiondetectiontestalgorithms
endif

motiondetectiontestalgorithms_SOURCES = testalgorithms.cpp
motiondetectiontestalgorithms_CPPFLAGS = -I$(top_srcdir)/src/libs -I..
motiondetectiontestalgorithms_LDADD = ../libmotiondetection.la

motiondetectiontestmodel_SOURCES = testmodel.cpp
motiondetectiontestmodel_CPPFLAGS = -I$(top_srcdir)/src/libs -I.. $(GEARBOX_CPPFLAGS)
motiondetectiontestmodel_LDADD = ../libmotiondetection.la

