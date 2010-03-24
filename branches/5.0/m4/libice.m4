dnl # Ice checks and variables definitions
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
if test "$ERRORS"; then
    AC_MSG_FAILURE([Errors found checking Ice support: $ERRORS.])
fi
AC_LANG_POP([C++])


AC_PATH_PROG([SLICE2CPP],[slice2cpp],[no])
if test "x$SLICE2CPP" = xno; then
    AC_MSG_FAILURE([could not find slice2cpp needed to build c++ interfaces code])
fi

AC_PATH_PROG([SLICE2JAVA],[slice2java],[no])
if test "x$SLICE2JAVA" = xno; then
    AC_MSG_FAILURE([could not find slice2java needed to build java interfaces code])
fi

    
AC_PATH_PROG([SLICE2PYTHON],[slice2py],[no])
if test "x$SLICE2PYTHON" = xno; then
    AC_MSG_FAILURE([could not find slice2py needed to build python interfaces code])
fi

