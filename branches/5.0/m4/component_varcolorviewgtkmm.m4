dnl # Requirements for component varcolorviewgtkmm
dnl # GTKmm

AC_ARG_ENABLE([component-varcolorviewgtkmm],
    [AS_HELP_STRING([--disable-component-varcolorviewgtkmm],
	    [disable varcolorviewgtkmm component compilation])],
    [],
    [enable_component_varcolorviewgtkmm=yes])


AM_CONDITIONAL([ENABLE_COMPONENT_VARCOLORVIEWGTKMM],[false])
if test "x$enable_component_varcolorviewgtkmm" != xno; then
    AC_MSG_NOTICE([**** Checking varcolorviewgtkmm component requirements:])
    PKG_CHECK_MODULES(
	[VARCOLORVIEWGTKMM],[gtkmm-2.4 libglademm-2.4 gthread-2.0],
	[
	    ENABLED_COMPONENTS="$ENABLED_COMPONENTS varcolorviewgtkmm"
	    AM_CONDITIONAL([ENABLE_COMPONENT_VARCOLORVIEWGTKMM],[true])
	],
	[
	    AC_MSG_FAILURE([--enable-component-varcolorviewgtkmm was given, but there was errors: $VARCOLORVIEWGTKMM_PKG_ERRORS])
	])
fi