dnl # Requirements for libcolorspacesmm
dnl # opencv


AC_MSG_NOTICE([**** Checking libcolorspacesmm requirements:])
if test "x$with_opencv" != xno; then
    AM_CONDITIONAL([ENABLE_LIBCOLORSPACESMM],[true])
    ENABLED_LIBCOLORSPACESMM="yes"
    AC_SUBST([LIBCOLORSPACESMM_CPPFLAGS],["$OPENCV_CPPFLAGS"])
    AC_SUBST([LIBCOLORSPACESMM_LDFLAGS],["$OPENCV_LDFLAGS"])
    AC_MSG_NOTICE([libcolorspacesmm enabled])
else
    AM_CONDITIONAL([ENABLE_LIBCOLORSPACESMM],[false])
    ENABLED_LIBCOLORSPACESMM="no"
    AC_MSG_NOTICE([libcolorspacesmm not enabled: opencv not found])
fi