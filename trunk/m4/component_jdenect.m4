dnl # Requirements for component jdenect
dnl # Gtkmm

AC_ARG_ENABLE([component-jdenect],
    [AS_HELP_STRING([--disable-component-jdenect],
	    [disable jdenect component compilation])],
    [],
    [enable_component_jdenect=yes])


if test "x$enable_component_jdenect" != xno; then
    AC_MSG_NOTICE([**** Checking jdenect component requirements:])
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking jdenect requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_JDENECT],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS jdenect"
	AM_CONDITIONAL([ENABLE_COMPONENT_JDENECT],[true])
    fi
fi
