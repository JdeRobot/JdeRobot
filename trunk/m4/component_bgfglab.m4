dnl # Requirements for component bgfglab
dnl # GTKmm, libcolorspacesmm, libbgfgsegmentation

AC_ARG_ENABLE([component-bgfglab],
    [AS_HELP_STRING([--disable-component-bgfglab],
	    [disable bgfglab component compilation])],
    [],
    [enable_component_bgfglab=yes])

if test "x$enable_component_bgfglab" != xno; then
    AC_MSG_NOTICE([**** Checking bgfglab component requirements:])
    ERRORS=""
    if test "x$with_colorspacesmm" = xno; then
	ERRORS="libcolorspacesmm not enabled"
    fi
    if test "x$with_gtkmm" = xno; then
	ERRORS="$ERRORS, gtkmm support not found"
    fi
    if test "x$with_bgfgsegmentation" = xno; then
	ERRORS="$ERRORS, libbgfgsegmentation not enabled"
    fi
    if test "$ERRORS"; then
        AC_MSG_NOTICE([Errors found checking bgfglab requirements: $ERRORS. Component disabled])
	AM_CONDITIONAL([ENABLE_COMPONENT_BGFGLAB],[false])
    else
	AC_MSG_NOTICE([Component enabled])
	ENABLED_COMPONENTS="$ENABLED_COMPONENTS bgfglab"
	AM_CONDITIONAL([ENABLE_COMPONENT_BGFGLAB],[true])
    fi
fi
