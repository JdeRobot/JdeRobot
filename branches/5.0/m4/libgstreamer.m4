dnl# Checks for gstreamer

AC_ARG_WITH([gstreamer],
    [AS_HELP_STRING([--with-gstreamer=<path>],[gstreamer library prefix path. Default /usr])],
    [],
    [with_gstreamer="/usr"])

if test "x$with_gstreamer" != xno; then
    AC_SUBST([gladedir],['${pkgdatadir}/glade'])
    GSTREAMER_PREFIX=$with_gstreamer
    GSTREAMER_INCLUDE="$GSTREAMER_PREFIX/include"
    GSTREAMER_CPPFLAGS="-I$GSTREAMER_INCLUDE"
    GSTREAMER_LDFLAGS="-L$GSTREAMER_PREFIX/lib"
    AC_SUBST([GSTREAMER_PREFIX])
    AC_SUBST([GSTREAMER_INCLUDE])
    AC_SUBST([GSTREAMER_CPPFLAGS])
    AC_SUBST([GSTREAMER_LDFLAGS])
                       
    _SAVE_CPPFLAGS=$CPPFLAGS
    _SAVE_LDFLAGS=$LDFLAGS
    CFLAGS="$CFLAGS $GSTREAMER_CFLAGS"
    CPPFLAGS="$CPPFLAGS $GSTREAMER_CPPFLAGS"
    LDFLAGS="$LDFLAGS $GSTREAMER_LDFLAGS"

    AC_MSG_NOTICE([**** Checking gstreamer support:])
    PKG_CHECK_MODULES(
	[GSTREAMER],[gstreamer-0.10 gstreamer-app-0.10],
	[
	    GSTREAMER_CPPFLAGS=$GSTREAMER_CFLAGS
	],
	[
	    AC_MSG_NOTICE([Errors found checking gstreamer support: $GSTREAMER_PKG_ERRORS. Try setting --with-gstreamer])
	    with_gstreamer="no"
	])

    CPPFLAGS=$_SAVE_CPPFLAGS
    LDFLAGS=$_SAVE_LDFLAGS
fi