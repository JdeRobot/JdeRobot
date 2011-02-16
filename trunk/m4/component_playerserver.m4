dnl # Requirements for component playerserver
dnl # Gtkmm

AC_ARG_ENABLE([component-playerserver],
    [AS_HELP_STRING([--disable-component-playerserver],
	    [disable playerserver component compilation])],
    [],
    [enable_component_playerserver=yes])


AM_CONDITIONAL([ENABLE_COMPONENT_PLAYERSERVER],[false])
if test "x$enable_component_playerserver" != xno; then
    AC_MSG_NOTICE([**** Checking playerserver component requirements:])
    if test "x$with_player" = xno; then
	ERRORS="$ERRORS, player support not found. Try setting --with-player"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking playerserver requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_PLAYERSERVER],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS playerserver"
	AM_CONDITIONAL([ENABLE_COMPONENT_PLAYERSERVER],[true])
    fi
fi
