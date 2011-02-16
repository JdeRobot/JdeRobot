dnl # Requirements for component alarmgenerator
dnl # GTKmm

AC_ARG_ENABLE([component-alarmgenerator],
    [AS_HELP_STRING([--disable-component-alarmgenerator],
	    [disable alarmgenerator component compilation])],
    [],
    [enable_component_alarmgenerator=yes])


AM_CONDITIONAL([ENABLE_COMPONENT_ALARMGENERATOR],[false])
