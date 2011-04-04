dnl # Requirements for component colortuner
dnl # GTKmm & opencv

AC_ARG_ENABLE([component-colortuner],
    [AS_HELP_STRING([--disable-component-colortuner],
	    [disable colortuner component compilation])],
    [],
    [enable_component_colortuner=yes])

AM_CONDITIONAL([ENABLE_COMPONENT_COLORTUNER],[false])
if test "x$enable_component_colortuner" != xno; then
    AC_MSG_NOTICE([**** Checking colortuner component requirements:])
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
        AC_MSG_NOTICE([Errors found checking colortuner requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_COLORTUNER],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS colortuner"
	AM_CONDITIONAL([ENABLE_COMPONENT_COLORTUNER],[true])
    fi
fi

