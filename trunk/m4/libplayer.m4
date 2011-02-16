dnl # Checks for player
dnl # If package found HAVE_PLAYER is defined in config header and with_player is /= no
dnl # PLAYER_CPPFLAGS and PLAYER_LDFLAGS are set as well.


AC_ARG_WITH([player],
    [AS_HELP_STRING([--with-player=<path>],[player library prefix path. Default /usr])],
    [],
    [with_player="/usr"])

if test "x$with_player" != xno; then
    PLAYER_CPPFLAGS="-I$with_player/include/player-2.1"
    PLAYER_LDFLAGS="-L$with_player/lib"
                       
    _SAVE_CPPFLAGS=$CPPFLAGS
    _SAVE_LDFLAGS=$LDFLAGS
    CPPFLAGS="$CPPFLAGS $PLAYER_CPPFLAGS"
    LDFLAGS="$LDFLAGS $PLAYER_LDFLAGS"

    PKG_CONFIG_PATH="$PKG_CONFIG_PATH:$with_player/lib/pkgconfig"
    AC_SUBST([PKG_CONFIG_PATH])

    AC_MSG_NOTICE([**** Checking player support:])
    PKG_CHECK_MODULES(
	[PLAYER],[playerc++],
	[
	    AC_DEFINE([HAVE_PLAYER],[1],[Defined if player found])
	    AC_SUBST([PLAYER_CPPFLAGS])
	    AC_SUBST([PLAYER_LDFLAGS],[$PLAYER_LIBS])
	],
	[
	    AC_MSG_NOTICE([Errors found checking player support: $PLAYER_PKG_ERRORS. Try setting --with-player])
	    with_player="no"
	])

    CPPFLAGS=$_SAVE_CPPFLAGS
    LDFLAGS=$_SAVE_LDFLAGS
fi
