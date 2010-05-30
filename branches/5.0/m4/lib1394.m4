dnl # Firewire checks and variables definitions
AC_MSG_NOTICE([**** Checking Ice support:])
AC_LANG_PUSH([C++])
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
    AM_CONDITIONAL([ENABLE_LIB1394],[false])
    ENABLED_LIB1394="no"
else
    AM_CONDITIONAL([ENABLE_LIB1394],[true])
    ENABLED_LIB1394="yes"
fi
AC_LANG_POP([C++])
