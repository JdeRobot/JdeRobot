dnl # Checks for opengl
dnl # If package found HAVE_OPENGL is defined in config header and with_opengl is /= no


AC_MSG_NOTICE([**** Checking opengl support:])
PKG_CHECK_MODULES(
    [OPENGL],[gl glu],
    [
	AC_DEFINE([HAVE_OPENGL],[1],[Defined if opengl found])
	AC_SUBST([OPENGL_CPPFLAGS],[$OPENGL_CFLAGS])
	AC_SUBST([OPENGL_LDFLAGS],[$OPENGL_LIBS])
    ],
    [
	AC_MSG_NOTICE([Errors found checking Opengl support: $OPENGL_PKG_ERRORS. Opengl support disabled])
	with_opengl="no"
    ])

ERRORS=""
AC_CHECK_HEADERS([GL/glut.h GL/freeglut.h],
    [],
    [ERRORS="$ac_header not found"]) 
AC_CHECK_LIB([glut],[main],
    [
	AC_DEFINE([HAVE_GLUT],[1],[Define if you have libglut])
    ],
    [ERRORS="$ERRORS, libglut not found"])

if test "$ERRORS"; then
    AC_MSG_NOTICE([Errors found checking Opengl support: $ERRORS. Opengl support disabled])
    with_opengl="no"
else
    AC_DEFINE([HAVE_OPENGL],[1],[Defined if opengl found])
    AC_SUBST([OPENGL_LDFLAGS],["$OPENGL_LDFLAGS -lglut"])
fi
