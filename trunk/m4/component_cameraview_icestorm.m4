dnl # Requirements for component cameraview_icestorm
dnl # GTKmm

AC_ARG_ENABLE([component-cameraview_icestorm],
    [AS_HELP_STRING([--disable-component-cameraview_icestorm],
	    [disable cameraview_icestorm component compilation])],
    [],
    [enable_component_cameraview_icestorm=yes])

if test "x$enable_component_cameraview_icestorm" != xno; then
    AC_MSG_NOTICE([**** Checking cameraview_icestorm component requirements:])
    ERRORS=""
    if test "x$with_colorspacesmm" = xno; then
	ERRORS="libcolorspacesmm not enabled"
    fi
    if test "x$with_gtkmm" = xno; then
	ERRORS="$ERRORS, gtkmm support not found"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking cameraview_icestorm requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_CAMERAVIEW_ICESTORM],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS cameraview_icestorm"
	AM_CONDITIONAL([ENABLE_COMPONENT_CAMERAVIEW_ICESTORM],[true])
    fi
fi
