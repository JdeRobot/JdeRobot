dnl # Requirements for component cameraserver
dnl # GStreamer
dnl # Firewire

AC_ARG_ENABLE([component-cameraserver],
    [AS_HELP_STRING([--disable-component-cameraserver],
	    [disable cameraserver component compilation])],
    [],
    [enable_component_cameraserver=yes])


if test "x$enable_component_cameraserver" != xno; then
    AC_MSG_NOTICE([**** Checking cameraserver component requirements:])
    if test "x$with_gstreamer" = xno; then
	ERRORS="$ERRORS, gstreamer support not found. Try setting --with-gstreamer"
    fi
    if test "x$with_firewire" = xno; then
	ERRORS="$ERRORS, firewire support not found."
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking cameraserver requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_CAMERASERVER],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS cameraserver"
	AM_CONDITIONAL([ENABLE_COMPONENT_CAMERASERVER],[true])
    fi
fi
