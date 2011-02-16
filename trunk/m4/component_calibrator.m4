dnl # Requirements for component calibrator
dnl # GTKmm

AC_ARG_ENABLE([component-calibrator],
    [AS_HELP_STRING([--disable-component-calibrator],
	    [disable calibrator component compilation])],
    [],
    [enable_component_calibrator=yes])

if test "x$enable_component_calibrator" != xno; then
    AC_MSG_NOTICE([**** Checking calibrator component requirements:])
    ERRORS=""
    if test "x$with_colorspacesmm" = xno; then
			ERRORS="libcolorspacesmm not enabled"
    fi
    if test "x$with_gtkmm" = xno; then
			ERRORS="$ERRORS, gtkmm support not found"
    fi
    if test "x$with_gtkglextmm" = xno; then
			ERRORS="$ERRORS, gtkglextmm support not found"
    fi
    if test "x$with_opencv" = xno; then
			ERRORS="$ERRORS, opencv support not found"
    fi
    if test "$ERRORS"; then
      AC_MSG_NOTICE([Errors found checking calibrator requirements: $ERRORS. Component disabled])
			AM_CONDITIONAL([ENABLE_COMPONENT_CALIBRATOR],[false])
    else
			AC_MSG_NOTICE([Component enabled])
			ENABLED_COMPONENTS="$ENABLED_COMPONENTS calibrator"
			AM_CONDITIONAL([ENABLE_COMPONENT_CALIBRATOR],[true])
    fi
fi
