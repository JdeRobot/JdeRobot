dnl # Requirements for component gazeboserver
dnl # Gtkmm

AC_ARG_ENABLE([component-gazeboserver],
    [AS_HELP_STRING([--disable-component-gazeboserver],
	    [disable gazeboserver component compilation])],
    [],
    [enable_component_gazeboserver=yes])


AM_CONDITIONAL([ENABLE_COMPONENT_GAZEBOSERVER],[false])
if test "x$enable_component_gazeboserver" != xno; then
    AC_MSG_NOTICE([**** Checking gazeboserver component requirements:])
    if test "x$with_gazebo" = xno; then
	ERRORS="$ERRORS, gazebo support not found. Try setting --with-gazebo"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking gazeboserver requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_GAZEBOSERVER],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS gazeboserver"
	AM_CONDITIONAL([ENABLE_COMPONENT_GAZEBOSERVER],[true])
    fi
fi
