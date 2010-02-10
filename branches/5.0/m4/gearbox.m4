dnl # Checks for gearbox

AC_ARG_WITH([gearbox],
    [AS_HELP_STRING([--with-gearbox=<path>],[gearbox library prefix path. Default /usr/local])],
    [],
    [with_gearbox="/usr/local"])

if test "x$with_gearbox" != xno; then
    GEARBOX_PREFIX=$with_gearbox
    GEARBOX_INCLUDE="$GEARBOX_PREFIX/include/gearbox"
    GEARBOX_CPPFLAGS="-I$GEARBOX_INCLUDE"
    GEARBOX_LDFLAGS="-L$GEARBOX_PREFIX/lib/gearbox"
    AC_SUBST([GEARBOX_PREFIX])
    AC_SUBST([GEARBOX_INCLUDE])
    AC_SUBST([GEARBOX_CPPFLAGS])
    AC_SUBST([GEARBOX_LDFLAGS])

    _SAVE_CPPFLAGS=$CPPFLAGS
    _SAVE_LDFLAGS=$LDFLAGS
    CPPFLAGS="$GEARBOX_CPPFLAGS"
    LDFLAGS="$GEARBOX_LDFLAGS"


    AC_MSG_NOTICE([**** Checking gearbox support:])
    AC_LANG_PUSH([C++])
    ERRORS=""
    AC_CHECK_HEADERS([gbxsickacfr/gbxiceutilacfr/thread.h gbxutilacfr/exceptions.h],
	[],
	[ERRORS="$ac_header not found"]) 
    AC_CHECK_LIB([GbxUtilAcfr],[main],
	[
	    AC_SUBST([LIBGBXUTILACFR],["-lGbxUtilAcfr"])
	    AC_DEFINE([HAVE_LIBGBXUTILACFR],[1],[Define if you have libGbxUtilAcfr])
	],
	[ERRORS="$ERRORS, libGbxUtilAcfr not found"])
    AC_CHECK_LIB([GbxIceUtilAcfr],[main],
	[
	    AC_SUBST([LIBGBXICEUTILACFR],["-lGbxIceUtilAcfr"])
	    AC_DEFINE([HAVE_LIBGBXICEUTILACFR],[1],[Define if you have libGbxIceUtilAcfr])
	],
	[ERRORS="$ERRORS, libGbxIceUtilAcfr not found"])
    if test "$ERRORS"; then
	AC_MSG_NOTICE([Errors found checking gearbox support: $ERRORS. Try setting --with-gearbox flag. Gearbox support disabled])
	with_gearbox="no"
    fi
    AC_LANG_POP([C++])
    CPPFLAGS=$_SAVE_CPPFLAGS
    LDFLAGS=$_SAVE_LDFLAGS
fi