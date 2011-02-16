dnl # Requirements for component recordingserver
dnl # mysql++

AC_ARG_ENABLE([component-recordingserver],
    [AS_HELP_STRING([--disable-component-recordingserver],
	    [disable recordingserver component compilation])],
    [],
    [enable_component_recordingserver=yes])


AM_CONDITIONAL([ENABLE_COMPONENT_RECORDINGSERVER],[false])
if test "x$enable_component_recordingserver" != xno; then
    AC_MSG_NOTICE([**** Checking recordingserver component requirements:])
    ERRORS=""
    AC_DEFINE([MYSQLPP_MYSQL_HEADERS_BURIED],[1],[Use headers under /usr/include/mysql])
    AC_LANG_PUSH([C++])
    AC_CHECK_HEADERS([mysql++/mysql++.h],
	[],
	[ERRORS="$ERRORS, $ac_header not found"])
    AC_CHECK_LIB([mysqlpp],[main],
	[RECORDINGSERVER_LIBS="$RECORDINGSERVER_LIBS -lmysqlpp"],
	[ERRORS="$ERRORS, libforms not found"])
    if test "$ERRORS"; then
	AC_MSG_FAILURE([--enable-component-recordingserver was given, but there was errors: $ERRORS])
    else
	ENABLED_COMPONENTS="$ENABLED_SCHEMASCOMPONENTS recordingserver"
	AM_CONDITIONAL([ENABLE_COMPONENT_RECORDINGSERVER],[true])
    fi
    AC_SUBST([RECORDINGSERVER_LIBS])
    AC_LANG_POP([C++])
fi
