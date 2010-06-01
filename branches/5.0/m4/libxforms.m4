dnl # Ice checks and variables definitions
AC_MSG_NOTICE([**** Checking Ice support:])
AC_LANG_PUSH([C++])
ERRORS=""
AC_CHECK_HEADERS([forms.h X11/Xlib.h X11/Xutil.h X11/Xos.h X11/Xatom.h],
    [],
    [ERRORS="$ac_header not found"]) 
AC_CHECK_LIB([X11],[main],
    [
	AC_SUBST([LIBX11],["-lX11"])
	AC_DEFINE([HAVE_LIBX11],[1],[Define if you have libX11])
    ],
    [ERRORS="$ERRORS, libX11 not found"])
AC_CHECK_LIB([forms],[main],
    [
	AC_SUBST([LIBFORMS],["-lforms"])
	AC_DEFINE([HAVE_LIBFORMS],[1],[Define if you have libforms])
    ],
    [ERRORS="$ERRORS, libforms not found"])
if test "$ERRORS"; then
    AC_MSG_FAILURE([Errors found checking xforms support: $ERRORS.])
    AM_CONDITIONAL([ENABLE_LIBXFORMS],[false])
    ENABLED_LIBXFORMS="no"
else
    AM_CONDITIONAL([ENABLE_LIBXFORMS],[true])
    ENABLED_LIBXFORMS="yes"
fi
AC_LANG_POP([C++])
