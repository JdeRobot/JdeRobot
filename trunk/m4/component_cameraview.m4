dnl # Requirements for component cameraview
dnl # GTKmm

AC_ARG_ENABLE([component-cameraview],
    [AS_HELP_STRING([--disable-component-cameraview],
	    [disable cameraview component compilation])],
    [],
    [enable_component_cameraview=yes])

if test "x$enable_component_cameraview" != xno; then
    AC_MSG_NOTICE([**** Checking cameraview component requirements:])
    ERRORS=""
    if test "x$with_colorspacesmm" = xno; then
	ERRORS="libcolorspacesmm not enabled"
    fi
    if test "x$with_gtkmm" = xno; then
	ERRORS="$ERRORS, gtkmm support not found"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking cameraview requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_CAMERAVIEW],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS cameraview"
	AM_CONDITIONAL([ENABLE_COMPONENT_CAMERAVIEW],[true])
    fi
fi