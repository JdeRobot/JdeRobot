dnl # Requirements for component calibrator

AC_ARG_ENABLE([component-calibrator],
    [AS_HELP_STRING([--disable-component-calibrator],
	    [disable calibrator component compilation])],
    [],
    [enable_component_calibrator=yes])

if test "x$enable_component_calibrator" != xno; then
    AC_MSG_NOTICE([**** Checking calibrator component requirements:])
    ERRORS=""
    if test "x$ENABLED_LIBCOLORSPACESMM" != xyes; then
	ERRORS="libcolorspacesmm not enabled"
    fi

    if test "x$ENABLED_LIBXFORMS" != xyes; then
	ERRORS="$ERRORS, libforms support not found."
    fi

    AC_PATH_PROG([FDESIGN],[fdesign],
	[ERRORS="$ERRORS, fdesign not found"])

    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking calibrator requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_CALIBRATOR],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS calibrator"
	AM_CONDITIONAL([ENABLE_COMPONENT_CALIBRATOR],[true])
    fi
fi
