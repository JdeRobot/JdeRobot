dnl # Checks for opencv
dnl # If package found HAVE_OPENCV is defined in config header and with_opencv is /= no
dnl # OPENCV_CPPFLAGS and OPENCV_LDFLAGS are set as well.


AC_ARG_WITH([opencv],
    [AS_HELP_STRING([--with-opencv=<path>],[opencv library prefix path. Default /usr])],
    [],
    [with_opencv="/usr"])

if test "x$with_opencv" != xno; then
    PKG_CONFIG_PATH="$PKG_CONFIG_PATH:$with_opencv/lib/pkgconfig"
    AC_SUBST([PKG_CONFIG_PATH])

    _SAVE_CPPFLAGS=$CPPFLAGS
    _SAVE_LDFLAGS=$LDFLAGS
    CPPFLAGS="$CPPFLAGS -I$with_opencv/include"
    LDFLAGS="$LDFLAGS -I$with_opencv/lib"

    AC_MSG_NOTICE([**** Checking opencv support:])
    PKG_CHECK_MODULES(
	[OPENCV],[opencv >= 2.0],
	[
	    AC_DEFINE([HAVE_OPENCV],[1],[Defined if OpenCv found])
	    AC_SUBST([OPENCV_CPPFLAGS],[$OPENCV_CFLAGS])
	    AC_SUBST([OPENCV_LDFLAGS],[$OPENCV_LIBS])
	],
	[         
	    AC_MSG_NOTICE([Errors found checking OpenCv support: $OPENCV_PKG_ERROR. Try setting --with-opencv])
	    with_opencv="no"
	])

    CPPFLAGS=$_SAVE_CPPFLAGS
    LDFLAGS=$_SAVE_LDFLAGS
fi
