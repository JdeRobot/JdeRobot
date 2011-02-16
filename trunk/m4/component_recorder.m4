dnl # Requirements for component recorder

AC_ARG_ENABLE([component-recorder],
    [AS_HELP_STRING([--disable-component-recorder],
	    [disable recorder component compilation])],
    [],
    [enable_component_recorder=yes])


AM_CONDITIONAL([ENABLE_COMPONENT_RECORDER],[false])
if test "x$enable_component_recorder" != xno; then
    AC_MSG_NOTICE([**** Checking recorder component requirements:])
    ERRORS=""
    
    AC_PATH_PROG([FFMPEG],[ffmpeg],[no])
    if test "x$FFMPEG" = xno; then
        ERRORS="$ERRORS, ffmpeg not found"
    fi

    if test "$ERRORS"; then
	AC_MSG_FAILURE([--enable-component-recorder was given, but there was errors: $ERRORS])
    else
	ENABLED_COMPONENTS="$ENABLED_SCHEMASCOMPONENTS recorder"
	AM_CONDITIONAL([ENABLE_COMPONENT_RECORDER],[true])
    fi
    AC_SUBST([RECORDER_LIBS])
fi
