if ENABLE_COMPONENT_INTROROB
bin_PROGRAMS = introrob
endif

introrob_SOURCES = introrob.cpp \
			introrobgui.cpp \
			introrobgui.h \
			navega.cpp \
			navega.h
introrob_CPPFLAGS = -I$(top_srcdir)/src/libs -I$(top_srcdir)/src/interfaces/cpp \
			$(GTKMM_CPPFLAGS) $(LIBCOLORSPACESMM_CPPFLAGS) \
			-DGLADE_DIR=\""$(gladedir)"\" \
			$(LIBGNOMEUI-DEV_CPPFLAGS)
introrob_LDFLAGS =  $(LIBICE) $(LIBICEUTIL) $(GTKMM_LDFLAGS) $(LIBGNOMEUI-DEV_LDFLAGS) $(LIBCOLORSPACESMM_LDFLAGS)
introrob_LDADD = $(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = introrob.cfg

