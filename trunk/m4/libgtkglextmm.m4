dnl # Checks for gtkglextmm
dnl # If package found HAVE_GTKGLEXTMM is defined in config header and with_gtkglextmm is /= no


AC_MSG_NOTICE([**** Checking gtkglextmm support:])
PKG_CHECK_MODULES(
    [GTKGLEXTMM],[gtkglextmm-1.2],
    [
	AC_DEFINE([HAVE_GTKGLEXTMM],[1],[Defined if gtkglextmm found])
	AC_SUBST([GTKGLEXTMM_CPPFLAGS],[$GTKGLEXTMM_CFLAGS])
	AC_SUBST([GTKGLEXTMM_LDFLAGS],[$GTKGLEXTMM_LIBS])
    ],
    [
	AC_MSG_NOTICE([Errors found checking Gtkglextmm support: $GTKGLEXTMM_PKG_ERRORS. Gtkglextmm support disabled])
	with_gtkglextmm="no"
    ])
