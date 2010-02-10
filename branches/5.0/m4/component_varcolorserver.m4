dnl # Requirements for component varcolorserver
dnl # GStreamer

AC_ARG_ENABLE([component-varcolorserver],
    [AS_HELP_STRING([--disable-component-varcolorserver],
	    [disable varcolorserver component compilation])],
    [],
    [enable_component_varcolorserver=yes])


if test "x$enable_component_varcolorserver" != xno; then
    AC_MSG_NOTICE([**** Checking varcolorserver component requirements:])
    if test "x$with_gstreamer" = xno; then
	ERRORS="$ERRORS, gstreamer support not found. Try setting --with-gstreamer"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking varcolorserver requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_VARCOLORSERVER],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS varcolorserver"
	AM_CONDITIONAL([ENABLE_COMPONENT_VARCOLORSERVER],[true])
    fi
fi