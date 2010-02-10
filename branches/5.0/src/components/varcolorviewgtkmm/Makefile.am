if ENABLE_COMPONENT_VARCOLORVIEWGTKMM
bin_PROGRAMS = varcolorviewgtkmm
endif

varcolorviewgtkmm_SOURCES = varcolorviewgtkmm.cpp \
				viewer.cpp \
				viewer.h
varcolorviewgtkmm_CXXFLAGS = $(VARCOLORVIEWGTKMM_CFLAGS) -I$(top_srcdir)/src/libs -I$(top_srcdir)/src/interfaces/cpp -DGLADE_DIR=$(gladedir)
varcolorviewgtkmm_LDFLAGS = $(VARCOLORVIEWGTKMM_LIBS)  $(LIBICE) $(LIBICEUTIL)
varcolorviewgtkmm_LDADD = $(top_srcdir)/src/libs/colorspaces/libcolorspaces.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspaces++.la \
			$(top_srcdir)/src/libs/colorspacesice/libcolorspacesice.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = varcolorviewgtkmm.cfg
dist_glade_DATA = varcolorviewgtkmm.glade