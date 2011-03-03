dnl # Checks for libfreenect
dnl # If package found HAVE_FREENECT is defined in config header and with_libfreenect is /= no


AC_MSG_NOTICE([**** Checking freenect support:])
PKG_CHECK_MODULES(
    [LIBFREENECT],[libfreenect],
    [
	AC_DEFINE([HAVE_FREENECT],[1],[Defined if freenect found])
	AC_SUBST([FREENECT_CPPFLAGS],[$FREENECT_CFLAGS])
	AC_SUBST([FREENECT_LDFLAGS],["-lfreenect"])
    ],
    [
	AC_MSG_NOTICE([Errors found checking Frenect support: $FREENECT_PKG_ERRORS. Freenect support disabled])
	with_freenect="no"
    ])
