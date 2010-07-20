dnl # Requirements for component introrob
dnl # GTKmm

AC_ARG_ENABLE([component-introrob],
    [AS_HELP_STRING([--disable-component-introrob],
	    [disable introrob component compilation])],
    [],
    [enable_component_introrob=yes])

if test "x$enable_component_introrob" != xno; then
    AC_MSG_NOTICE([**** Checking introrob component requirements:])
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
        AC_MSG_NOTICE([Errors found checking introrob requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_INTROROB],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS introrob"
	AM_CONDITIONAL([ENABLE_COMPONENT_INTROROB],[true])
    fi
fi
