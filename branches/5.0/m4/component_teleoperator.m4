dnl # Requirements for component teleoperator
dnl # GTKmm

AC_ARG_ENABLE([component-teleoperator],
    [AS_HELP_STRING([--disable-component-teleoperator],
	    [disable teleoperator component compilation])],
    [],
    [enable_component_teleoperator=yes])

if test "x$enable_component_teleoperator" != xno; then
    AC_MSG_NOTICE([**** Checking teleoperator component requirements:])
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
    if test "x$with_opengl" = xno; then
			ERRORS="$ERRORS, opengl support not found"
    fi
    if test "x$with_gsl" = xno; then
			ERRORS="$ERRORS, gsl support not found"
    fi
    if test "$ERRORS"; then
      AC_MSG_NOTICE([Errors found checking teleoperator requirements: $ERRORS. Component disabled])
			AM_CONDITIONAL([ENABLE_COMPONENT_TELEOPERATOR],[false])
    else
			AC_MSG_NOTICE([Component enabled])
			ENABLED_COMPONENTS="$ENABLED_COMPONENTS teleoperator"
			AM_CONDITIONAL([ENABLE_COMPONENT_TELEOPERATOR],[true])
    fi
fi
