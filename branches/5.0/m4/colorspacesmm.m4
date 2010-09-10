dnl # Requirements for libcolorspacesmm
dnl # If lib enabled HAVE_COLORSPACESMM is defined in config header and with_colorspaces will be /= no
dnl # COLORSPACES_CPPFLAGS and COLORSPACES_LDFLAGS are set as well.

AC_MSG_NOTICE([**** Checking libcolorspacesmm requirements:])
if test "x$with_opencv" != xno; then
    AM_CONDITIONAL([ENABLE_LIBCOLORSPACESMM],[true])
    with_colorspacesmm="yes"
    AC_DEFINE([HAVE_COLORSPACESMM],[1],[Defined if colorspacesmm enabled])
    AC_SUBST([COLORSPACESMM_CPPFLAGS],["$OPENCV_CPPFLAGS -I\${top_srcdir}/src/libs"])
    AC_SUBST([COLORSPACESMM_LDFLAGS],["$OPENCV_LDFLAGS"])
    AC_MSG_NOTICE([libcolorspacesmm enabled])
else
    AM_CONDITIONAL([ENABLE_LIBCOLORSPACESMM],[false])
    with_colorspacesmm="no"
    AC_MSG_NOTICE([libcolorspacesmm not enabled: opencv not found])
fi
