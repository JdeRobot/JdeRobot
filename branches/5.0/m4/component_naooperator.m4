dnl # Requirements for component naooperator
dnl # Gtkmm

AC_ARG_ENABLE([component-naooperator],
    [AS_HELP_STRING([--disable-component-naooperator],
	    [disable naooperator component compilation])],
    [],
    [enable_component_naooperator=yes])


if test "x$enable_component_naooperator" != xno; then
    AC_MSG_NOTICE([**** Checking naooperator component requirements:])
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking naooperator requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_NAOOPERATOR],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS naooperator"
	AM_CONDITIONAL([ENABLE_COMPONENT_NAOOPERATOR],[true])
    fi
fi
