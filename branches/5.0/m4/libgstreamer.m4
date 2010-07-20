dnl# Checks for gstreamer
dnl # If package found HAVE_GSTREAMER is defined in config header and with_gstreamer is /= no

AC_MSG_NOTICE([**** Checking gstreamer support:])
PKG_CHECK_MODULES(
    [GSTREAMER],[gstreamer-0.10 gstreamer-app-0.10],
    [
	AC_DEFINE([HAVE_GSTREAMER],[1],[Defined if gstreamer found])
	AC_SUBST([GSTREAMER_CPPFLAGS],[$GSTREAMER_CFLAGS])
	AC_SUBST([GSTREAMER_LDFLAGS],[$GSTREAMER_LIBS])
    ],
    [
	AC_MSG_NOTICE([Errors found checking gstreamer support: $GSTREAMER_PKG_ERRORS. Gstreamer support diabled])
	with_gstreamer="no"
    ])
