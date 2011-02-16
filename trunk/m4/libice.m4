dnl # Ice checks and variables definitions
dnl # If package found HAVE_ICE is defined in config header and with_ice is /= no

AC_MSG_NOTICE([**** Checking Ice support:])
AC_LANG_PUSH([C++])
ERRORS=""
AC_CHECK_HEADERS([Ice/Ice.h IceUtil/IceUtil.h IceBox/IceBox.h IceStorm/IceStorm.h],
    [],
    [ERRORS="$ac_header not found"]) 
AC_CHECK_LIB([Ice],[main],
    [
	AC_SUBST([LIBICE],["-lIce"])
	AC_DEFINE([HAVE_LIBICE],[1],[Define if you have libIce])
    ],
    [ERRORS="$ERRORS, libIce not found"])
AC_CHECK_LIB([IceUtil],[main],
    [
	AC_SUBST([LIBICEUTIL],["-lIceUtil"])
	AC_DEFINE([HAVE_LIBICEUTIL],[1],[Define if you have libIceUtil])
    ],
    [ERRORS="$ERRORS, libIceUtil not found"])
AC_CHECK_LIB([IceGrid],[main],
    [
	AC_SUBST([LIBICEGRID],["-lIceGrid"])
	AC_DEFINE([HAVE_LIBICEGRID],[1],[Define if you have libIceGrid])
    ],
    [ERRORS="$ERRORS, libIceGrid not found"])
AC_CHECK_LIB([IceBox],[main],
    [
	AC_SUBST([LIBICEBOX],["-lIceBox"])
	AC_DEFINE([HAVE_LIBICEBOX],[1],[Define if you have libIceBox])
    ],
    [ERRORS="$ERRORS, libIceBox not found"])
AC_CHECK_LIB([IceStorm],[main],
    [
	AC_SUBST([LIBICESTORM],["-lIceStorm"])
	AC_DEFINE([HAVE_LIBICESTORM],[1],[Define if you have libIceStorm])
    ],
    [ERRORS="$ERRORS, libIceStorm not found"])
AC_CHECK_LIB([IceStormService],[main],
    [
	AC_SUBST([LIBICESTORMSERVICE],["-lIceStormService"])
	AC_DEFINE([HAVE_LIBICESTORMSERVICE],[1],[Define if you have libIceStormService])
    ],
    [ERRORS="$ERRORS, libIceStormService not found"])
AC_LANG_POP([C++])

AC_ARG_VAR([SLICE2CPP],[command use to generate c++ code from slice])
AC_ARG_VAR([SLICE2JAVA],[command use to generate java code from slice])
AC_ARG_VAR([SLICE2PYTHON],[command use to generate python code from slice])


AC_PATH_PROG([SLICE2CPP],[slice2cpp],[no])
if test "x$SLICE2CPP" = xno; then
    ERRORS="$ERRORS, could not find slice2cpp needed to build c++ interfaces code"
fi

AC_PATH_PROG([SLICE2JAVA],[slice2java],[no])
if test "x$SLICE2JAVA" = xno; then
    ERRORS="$ERRORS, could not find slice2java needed to build java interfaces code"
fi

AC_PATH_PROG([SLICE2PYTHON],[slice2py],[no])
if test "x$SLICE2PYTHON" = xno; then
    ERRORS="$ERRORS, could not find slice2py needed to build python interfaces code"
fi

dnl #define SLICEDIR if empty
if test -z "$SLICEDIR"; then
   SLICEDIR="/usr/share/slice"
fi

if test "$ERRORS"; then
    AC_MSG_NOTICE([Errors found checking Ice support: $ERRORS. Ice support disabled])
    with_ice="no"
else
    AC_DEFINE([HAVE_ICE],[1],[Defined if Ice found])
    AC_SUBST([ICE_CPPFLAGS])
    AC_SUBST([ICE_LDFLAGS],["$LIBICE $LIBICEUTIL $LIBICEGRID $LIBICEBOX $LIBICESTORM $LIBICESTORMSERVICE"])
fi
