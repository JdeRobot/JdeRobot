dnl # Requirements for component surveillance 
dnl # FIXME: check dependencies
AC_ARG_ENABLE([component-surveillance],
    [AS_HELP_STRING([--disable-component-surveillance],
	    [disable surveillance component compilation])],
    [],
    [enable_component_surveillance=yes])


AM_CONDITIONAL([ENABLE_COMPONENT_SURVEILLANCE],[false])
if test "x$enable_component_surveillance" != xno; then
    AC_MSG_NOTICE([**** Checking surveillance component requirements:])
    ERRORS=""
    

    if test "$ERRORS"; then
	AC_MSG_FAILURE([--enable-component-surveillance was given, but there was errors: $ERRORS])
    else
	ENABLED_COMPONENTS="$ENABLED_SCHEMASCOMPONENTS surveillance"
	AM_CONDITIONAL([ENABLE_COMPONENT_SURVEILLANCE],[true])
    fi
    AC_SUBST([SURVEILLANCE_LIBS])
fi
