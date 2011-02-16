dnl # Requirements for libjderobotice
dnl # Ice & Gearbox


AC_MSG_NOTICE([**** Checking libjderobotice requirements:])
if test "x$with_ice" = xno; then
    AM_CONDITIONAL([ENABLE_LIBJDEROBOTICE],[false])
    with_jderobotice="no"
    AC_MSG_NOTICE([libjderobotice not enabled: Ice not found])
elif test "x$with_gearbox" = xno; then
    AM_CONDITIONAL([ENABLE_LIBJDEROBOTICE],[false])
    with_jderobotice="no"
    AC_MSG_NOTICE([libjderobotice not enabled: gearbox not found])
else
    AM_CONDITIONAL([ENABLE_LIBJDEROBOTICE],[true])
    with_jderobotice="yes"
    AC_SUBST([JDEROBOTICE_CPPFLAGS],["$ICE_CPPFLAGS $GEARBOX_CPPFLAGS -I\${top_srcdir}/src/libs -I\${top_srcdir}/src/interfaces/cpp"])
    AC_SUBST([JDEROBOTICE_LDFLAGS],["$ICE_LDFLAGS $GEARBOX_LDFLAGS"])
    AC_MSG_NOTICE([libjderobotice enabled])
fi

