dnl # Requirements for component <component-name>
dnl #

AC_ARG_ENABLE([component-<component-name>],
    [AS_HELP_STRING([--disable-component-<component-name>],
	    [disable <component-name> component compilation])],
    [],
    [enable_component_<component-name>=yes])


AM_CONDITIONAL([ENABLE_COMPONENT_<component-name>],[false])
if test "x$enable_component_<component-name>" != xno; then
    AC_MSG_NOTICE([**** Checking <component-name> component requirements:])
    dnl # checks
fi