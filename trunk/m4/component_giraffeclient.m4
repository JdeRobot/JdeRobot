dnl # Requirements for component giraffeclient
dnl # GTKmm

AC_ARG_ENABLE([component-giraffeclient],
    [AS_HELP_STRING([--disable-component-giraffeclient],
	    [disable giraffeclient component compilation])],
    [],
    [enable_component_giraffeclient=yes])

if test "x$enable_component_giraffeclient" != xno; then
    AC_MSG_NOTICE([**** Checking giraffeclient component requirements:])
    ERRORS=""
    if test "x$with_colorspacesmm" = xno; then
			ERRORS="libcolorspacesmm not enabled"
    fi
    if test "x$with_gtkmm" = xno; then
			ERRORS="$ERRORS, gtkmm support not found"
    fi
    if test "x$with_gtkglextmm" = xno; then
			ERRORS="$ERRORS, gtkglextmm support not found"
    fi
    if test "x$with_opencv" = xno; then
			ERRORS="$ERRORS, opencv support not found"
    fi
    if test "x$with_gsl" = xno; then
			ERRORS="$ERRORS, gsl support not found"
    fi
    if test "$ERRORS"; then
      AC_MSG_NOTICE([Errors found checking giraffeclient requirements: $ERRORS. Component disabled])
			AM_CONDITIONAL([ENABLE_COMPONENT_GIRAFFECLIENT],[false])
    else
			AC_MSG_NOTICE([Component enabled])
			ENABLED_COMPONENTS="$ENABLED_COMPONENTS giraffeclient"
			AM_CONDITIONAL([ENABLE_COMPONENT_GIRAFFECLIENT],[true])
    fi
fi
