dnl # Checks for gtkmm

AC_ARG_WITH([gtkmm],
    [AS_HELP_STRING([--with-gtkmm=<path>],[gtkmm library prefix path. Default /usr])],
    [],
    [with_gtkmm="/usr"])

if test "x$with_gtkmm" != xno; then
    AC_SUBST([gladedir],['${pkgdatadir}/glade'])
    GTKMM_PREFIX=$with_gtkmm
    GTKMM_INCLUDE="$GTKMM_PREFIX/include"
    GTKMM_CPPFLAGS="-I$GTKMM_INCLUDE"
    GTKMM_LDFLAGS="-L$GTKMM_PREFIX/lib"
    AC_SUBST([GTKMM_PREFIX])
    AC_SUBST([GTKMM_INCLUDE])
    AC_SUBST([GTKMM_CPPFLAGS])
    AC_SUBST([GTKMM_LDFLAGS])
                       
    _SAVE_CPPFLAGS=$CPPFLAGS
    _SAVE_LDFLAGS=$LDFLAGS
    CFLAGS="$CFLAGS $GTKMM_CFLAGS"
    CPPFLAGS="$CPPFLAGS $GTKMM_CPPFLAGS"
    LDFLAGS="$LDFLAGS $GTKMM_LDFLAGS"

    AC_MSG_NOTICE([**** Checking gtkmm support:])
    PKG_CHECK_MODULES(
	[GTKMM],[gtkmm-2.4 libglademm-2.4 gthread-2.0],
	[
	    GTKMM_CPPFLAGS=$GTKMM_CFLAGS
	    GTKMM_LDFLAGS=$GTKMM_LIBS
	],
	[
	    AC_MSG_NOTICE([Errors found checking Gtkmm support: $GTKMM_PKG_ERRORS. Try setting --with-gtkmm])
	    with_gtkmm="no"
	])

    CPPFLAGS=$_SAVE_CPPFLAGS
    LDFLAGS=$_SAVE_LDFLAGS
fi