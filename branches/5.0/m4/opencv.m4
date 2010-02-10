dnl # Checks for opencv

AC_ARG_WITH([opencv],
    [AS_HELP_STRING([--with-opencv=<path>],[opencv library prefix path. Default /usr/local])],
    [],
    [with_opencv="/usr/local"])

if test "x$with_opencv" != xno; then
    OPENCV_PREFIX=$with_opencv
    OPENCV_INCLUDE="$OPENCV_PREFIX/include"
    OPENCV_CPPFLAGS="-I$OPENCV_INCLUDE"
    OPENCV_LDFLAGS="-L$OPENCV_PREFIX/lib"
    AC_SUBST([OPENCV_PREFIX])
    AC_SUBST([OPENCV_INCLUDE])
    AC_SUBST([OPENCV_CPPFLAGS])
    AC_SUBST([OPENCV_LDFLAGS])
                       
    _SAVE_CPPFLAGS=$CPPFLAGS
    _SAVE_LDFLAGS=$LDFLAGS
    CFLAGS="$CFLAGS $OPENCV_CFLAGS"
    CPPFLAGS="$CPPFLAGS $OPENCV_CPPFLAGS"
    LDFLAGS="$LDFLAGS $OPENCV_LDFLAGS"

    AC_MSG_NOTICE([**** Checking opencv support:])
    ERRORS=""
    AC_CHECK_HEADERS([opencv/cv.h opencv/highgui.h],
        [],
        [ERRORS="$ERRORS, $ac_header not found"])
    AC_CHECK_LIB([cxcore],[main], 
        [OPENCV_LDFLAGS="$OPENCV_LDFLAGS -lcxcore"],
        [ERRORS="$ERRORS, cxcore library not found"])
    AC_CHECK_LIB([cv],[main], 
        [OPENCV_LDFLAGS="$OPENCV_LDFLAGS -lcv"],
        [ERRORS="$ERRORS, cv library not found"])
dnl# highgui use img_convert from libavcodec that has been deprecated in new versions
dnl#    AC_CHECK_LIB([highgui],[main], 
dnl#        [OPENCV_LDFLAGS="$OPENCV_LDFLAGS -lhighgui"],
dnl#        [ERRORS="$ERRORS, highgui library not found"])
    AC_CHECK_LIB([cvaux],[main], 
        [OPENCV_LDFLAGS="$OPENCV_LDFLAGS -lcvaux"],
        [ERRORS="$ERRORS, cvaux library not found"])
    AC_CHECK_LIB([ml],[main], 
        [OPENCV_LDFLAGS="$OPENCV_LDFLAGS -lml"],
        [ERRORS="$ERRORS, ml library not found"])

    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking opencv support: $ERRORS. Try setting --with-opencv])
	with_opencv="no"
    fi
    CPPFLAGS=$_SAVE_CPPFLAGS
    LDFLAGS=$_SAVE_LDFLAGS

fi