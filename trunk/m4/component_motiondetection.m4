dnl # Requirements for component motiondetection
dnl # GTKmm,opencv

AC_ARG_ENABLE([component-motiondetection],
    [AS_HELP_STRING([--enable-component-motiondetection],
	    [enable motiondetection component compilation])],
    [],
    [enable_component_motiondetection=no])

if test "x$enable_component_motiondetection" != xno; then
    AC_MSG_NOTICE([**** Checking motiondetection component requirements:])
    ERRORS=""
    if test "x$with_jderobotice" = xno; then
	ERRORS="libjderobotice not enabled"
    fi
    if test "x$with_opencv" = xno; then
	ERRORS="$ERRORS, opencv support not found. Try setting --with-opencv"
    fi
    if test "x$with_gtkmm" = xno; then
	ERRORS="$ERRORS, gtkmm support not found"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking motiondetection requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_MOTIONDETECTION],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS motiondetection"
	AM_CONDITIONAL([ENABLE_COMPONENT_MOTIONDETECTION],[true])
    fi
else
    AM_CONDITIONAL([ENABLE_COMPONENT_MOTIONDETECTION],[false])
fi