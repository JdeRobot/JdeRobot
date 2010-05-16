dnl# Checks for gazebo

AC_ARG_WITH([gazebo],
    [AS_HELP_STRING([--with-gazebo=<path>],[gazebo library prefix path. Default /usr])],
    [],
    [with_gazebo="/usr"])

if test "x$with_gazebo" != xno; then
dnl#    AC_SUBST([gladedir],['${pkgdatadir}/glade'])
    GAZEBO_PREFIX=$with_gstreamer
    GAZEBO_INCLUDE="$GAZEBO_PREFIX/include"
    GAZEBO_CPPFLAGS="-I$GAZEBO_INCLUDE"
dnl#    GAZEBO_LDFLAGS="-L$GAZEBO_PREFIX/lib"
    AC_SUBST([GAZEBO_PREFIX])
    AC_SUBST([GAZEBO_INCLUDE])
    AC_SUBST([GAZEBO_CPPFLAGS])
dnl#    AC_SUBST([GAZEBO_LDFLAGS])
                       
    _SAVE_CPPFLAGS=$CPPFLAGS
dnl#    _SAVE_LDFLAGS=$LDFLAGS
    CFLAGS="$CFLAGS $GAZEBO_CFLAGS"
    CPPFLAGS="$CPPFLAGS $GAZEBO_CPPFLAGS"
dnl#    LDFLAGS="$LDFLAGS $GAZEBO_LDFLAGS"

    AC_MSG_NOTICE([**** Checking gazebo support:])
    PKG_CHECK_MODULES(
	[GAZEBO],[gazebo],
	[
	    GAZEBO_CPPFLAGS=$GAZEBO_CFLAGS
dnl#	    GAZEBO_LDFLAGS=$GAZEBO_LIBS
	],
	[
	    AC_MSG_NOTICE([Errors found checking gazebo support: $GAZEBO_PKG_ERRORS. Try setting --with-gazebo])
	    with_gazebo="no"
	])

    CPPFLAGS=$_SAVE_CPPFLAGS
dnl#    LDFLAGS=$_SAVE_LDFLAGS
fi
