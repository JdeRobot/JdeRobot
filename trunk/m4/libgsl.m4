dnl # Checks for gsl
dnl # If package found HAVE_GSL is defined in config header and with_gsl is /= no


AC_MSG_NOTICE([**** Checking gsl support:])
PKG_CHECK_MODULES(
    [GSL],[gsl],
    [
	AC_DEFINE([HAVE_GSL],[1],[Defined if gsl found])
	AC_SUBST([GSL_CPPFLAGS],[$GSL_CFLAGS])
	AC_SUBST([GSL_LDFLAGS],[$GSL_LIBS])
    ],
    [
	AC_MSG_NOTICE([Errors found checking Gsl support: $GSL_PKG_ERRORS. Gsl support disabled])
	with_gsl="no"
    ])
