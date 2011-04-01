dnl # Requirements for component opencvdemo
dnl # GTKmm & opencv

AC_ARG_ENABLE([component-opencvdemo],
    [AS_HELP_STRING([--disable-component-opencvdemo],
	    [disable opencvdemo component compilation])],
    [],
    [enable_component_opencvdemo=yes])

AM_CONDITIONAL([ENABLE_COMPONENT_OPENCVDEMO],[false])
if test "x$enable_component_opencvdemo" != xno; then
    AC_MSG_NOTICE([**** Checking opencvdemo component requirements:])
    ERRORS=""
    if test "x$with_colorspacesmm" = xno; then
	ERRORS="libcolorspacesmm not enabled"
    fi
    if test "x$with_gtkmm" = xno; then
	ERRORS="$ERRORS, gtkmm support not found"
    fi
    if test "x$with_opencv" = xno; then
	ERRORS="$ERRORS, opencv support not found. Try setting --with-opencv"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking opencvdemo requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_OPENCVDEMO],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS opencvdemo"
	AM_CONDITIONAL([ENABLE_COMPONENT_OPENCVDEMO],[true])
    fi
fi

