dnl # Checks for gazebo
dnl # If package found HAVE_GAZEBO is defined in config header and with_gazebo is /= no
dnl # GAZEBO_CPPFLAGS and GAZEBO_LDFLAGS are set as well.


AC_ARG_WITH([gazebo],
    [AS_HELP_STRING([--with-gazebo=<path>],[gazebo library prefix path. Default /usr])],
    [],
    [with_gazebo="/usr"])

if test "x$with_gazebo" != xno; then
    GAZEBO_CPPFLAGS="-I$with_gazebo/include"
    GAZEBO_LDFLAGS="-L$with_gazebo/lib"
                       
    _SAVE_CPPFLAGS=$CPPFLAGS
    _SAVE_LDFLAGS=$LDFLAGS
    CPPFLAGS="$CPPFLAGS $GAZEBO_CPPFLAGS"
    LDFLAGS="$LDFLAGS $GAZEBO_LDFLAGS"

    PKG_CONFIG_PATH="$PKG_CONFIG_PATH:$with_gazebo/lib/pkgconfig"
    AC_SUBST([PKG_CONFIG_PATH])

    AC_MSG_NOTICE([**** Checking gazebo support:])
    PKG_CHECK_MODULES(
	[GAZEBO],[libgazebo],
	[
	    AC_DEFINE([HAVE_GAZEBO],[1],[Defined if gazebo found])
	    AC_SUBST([GAZEBO_CPPFLAGS])
	    AC_SUBST([GAZEBO_LDFLAGS],[$GAZEBO_LIBS])
	],
	[
	    AC_MSG_NOTICE([Errors found checking gazebo support: $GAZEBO_PKG_ERRORS. Try setting --with-gazebo])
	    with_gazebo="no"
	])

    CPPFLAGS=$_SAVE_CPPFLAGS
    LDFLAGS=$_SAVE_LDFLAGS
fi
