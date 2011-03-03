dnl # Requirements for component visornect
dnl # GTKmm

AC_ARG_ENABLE([component-visornect],
    [AS_HELP_STRING([--disable-component-visornect],
	    [disable visornect component compilation])],
    [],
    [enable_component_visornect=yes])

if test "x$enable_component_visornect" != xno; then
    AC_MSG_NOTICE([**** Checking visornect component requirements:])
    ERRORS=""
    if test "x$with_colorspacesmm" = xno; then
	ERRORS="libcolorspacesmm not enabled"
    fi
    if test "x$with_gtkmm" = xno; then
	ERRORS="$ERRORS, gtkmm support not found"
    fi
    dnl # FIXME: what is libgnomeui-dev and where is defined?
    if test "x$with_libgnomeui-dev" = xno; then
	ERRORS="$ERRORS, libgnomeui-dev support not found. Try setting --with-libgnomeui-dev"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking visornect requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_VISORNECT],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS visornect"
	AM_CONDITIONAL([ENABLE_COMPONENT_VISORNECT],[true])
    fi
fi
