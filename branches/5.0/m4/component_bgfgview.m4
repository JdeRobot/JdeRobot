dnl # Requirements for component bgfgview
dnl # GTKmm, libcolorspacesmm, libbgfgfilters

AC_ARG_ENABLE([component-bgfgview],
    [AS_HELP_STRING([--disable-component-bgfgview],
	    [disable bgfgview component compilation])],
    [],
    [enable_component_bgfgview=yes])

if test "x$enable_component_bgfgview" != xno; then
    AC_MSG_NOTICE([**** Checking bgfgview component requirements:])
    ERRORS=""
    if test "x$with_colorspacesmm" = xno; then
	ERRORS="libcolorspacesmm not enabled"
    fi
    if test "x$with_gtkmm" = xno; then
	ERRORS="$ERRORS, gtkmm support not found"
    fi
    if test "x$with_bgfgfilters" = xno; then
	ERRORS="$ERRORS, libbgfgfilters not enabled"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking bgfgview requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_BGFGVIEW],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS bgfgview"
	AM_CONDITIONAL([ENABLE_COMPONENT_BGFGVIEW],[true])
    fi
fi