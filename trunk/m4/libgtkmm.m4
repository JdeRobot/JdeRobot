dnl # Checks for gtkmm
dnl # If package found HAVE_GTKMM is defined in config header and with_gtkmm is /= no

AC_SUBST([gladedir],['${pkgdatadir}/glade'])

AC_MSG_NOTICE([**** Checking gtkmm support:])
PKG_CHECK_MODULES(
    [GTKMM],[gtkmm-2.4 libglademm-2.4 gthread-2.0],
    [
	AC_DEFINE([HAVE_GTKMM],[1],[Defined if gtkmm found])
	AC_SUBST([GTKMM_CPPFLAGS],[$GTKMM_CFLAGS])
	AC_SUBST([GTKMM_LDFLAGS],[$GTKMM_LIBS])
    ],
    [
	AC_MSG_NOTICE([Errors found checking Gtkmm support: $GTKMM_PKG_ERRORS. Gtkmm support disabled])
	with_gtkmm="no"
    ])
