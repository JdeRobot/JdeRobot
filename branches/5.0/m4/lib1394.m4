dnl # Firewire support checks and variables definitions
dnl # If package found HAVE_FIREWIRE is defined in config header and with_firewire is /= no

AC_MSG_NOTICE([**** Checking firewire support:])
ERRORS=""
AC_CHECK_HEADERS([libraw1394/raw1394.h dc1394/control.h],
    [],
    [ERRORS="$ac_header not found"]) 
AC_CHECK_LIB([dc1394],[main],
    [
	AC_SUBST([LIBDC1394],["-ldc1394"])
	AC_DEFINE([HAVE_LIBDC1394],[1],[Define if you have libdc1394])
    ],
    [ERRORS="$ERRORS, libdc1394 not found"])
AC_CHECK_LIB([raw1394],[main],
    [
	AC_SUBST([LIBRAW1394],["-lraw1394"])
	AC_DEFINE([HAVE_LIBRAW1394],[1],[Define if you have libraw1394])
    ],
    [ERRORS="$ERRORS, libraw1394 not found"])
if test "$ERRORS"; then
    AC_MSG_FAILURE([Errors found checking firewire support: $ERRORS.])
    with_firewire="no"
else
    AC_DEFINE([HAVE_FIREWIRE],[1],[Defined if firewire found])
    AC_SUBST([FIREWIRE_CPPFLAGS])
    AC_SUBST([FIREWIRE_LDFLAGS],["-ldc1394 -lraw1394"])
fi
