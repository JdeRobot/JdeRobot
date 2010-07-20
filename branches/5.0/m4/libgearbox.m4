dnl # Checks for gearbox
dnl # If package found HAVE_GEARBOX is defined in config header and with_gearbox will be /= no
dnl # GEARBOX_CPPFLAGS and GEARBOX_LDFLAGS are set as well.

AC_ARG_WITH([gearbox],
    [AS_HELP_STRING([--with-gearbox=<path>],[gearbox library prefix path. Default /usr])],
    [],
    [with_gearbox="/usr"])

if test "x$with_gearbox" != xno; then
    GEARBOX_CPPFLAGS="-I$with_gearbox/include/gearbox"
    GEARBOX_LDFLAGS="-L$with_gearbox/lib/gearbox"
    
    _SAVE_CPPFLAGS=$CPPFLAGS
    _SAVE_LDFLAGS=$LDFLAGS
    CPPFLAGS="$CPPFLAGS $GEARBOX_CPPFLAGS"
    LDFLAGS="$LDFLAGS $GEARBOX_LDFLAGS"
    
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
    else
	AC_DEFINE([HAVE_GEARBOX],[1],[Defined if gearbox found])
	AC_SUBST([GEARBOX_CPPFLAGS])
	AC_SUBST([GEARBOX_LDFLAGS],["$GEARBOX_LDFLAGS $LIBGBXUTILACFR $LIBGBXICEUTILACFR"])
    fi
    AC_LANG_POP([C++])

    CPPFLAGS=$_SAVE_CPPFLAGS
    LDFLAGS=$_SAVE_LDFLAGS
fi
