dnl # Checks for opencv

AC_ARG_WITH([opencv],
    [AS_HELP_STRING([--with-opencv=YES|no],[opencv library. Default yes])],
    [],
    [with_opencv="yes"])

if test "x$with_opencv" != xno; then
    AC_MSG_NOTICE([**** Checking opencv support:])
    PKG_CHECK_MODULES(
        [OPENCV],[opencv >= 2.0],
        [
            OPENCV_CPPFLAGS=$OPENCV_CFLAGS
            OPENCV_LDFLAGS=$OPENCV_LIBS
        ],
        [         
	    AC_MSG_NOTICE([Errors found checking Gtkmm support: $OPENCV_PKG_ERROR
S. Try setting --with-opencv])
            with_opencv="no"
        ])
fi