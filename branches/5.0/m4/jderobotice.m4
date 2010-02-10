dnl # Requirements for libjderobotice
dnl # Gearbox


AC_MSG_NOTICE([**** Checking libjderobotice requirements:])
if test "x$with_gearbox" != xno; then
    AM_CONDITIONAL([ENABLE_LIBJDEROBOTICE],[true])
    ENABLED_LIBJDEROBOTICE="yes"
    AC_SUBST([LIBJDEROBOTICE_CPPFLAGS],["$GEARBOX_CPPFLAGS -I `pwd`/src/libs"])
    AC_MSG_NOTICE([libjderobotice enabled])
else
    AM_CONDITIONAL([ENABLE_LIBJDEROBOTICE],[false])
    ENABLED_LIBJDEROBOTICE="no"
    AC_MSG_NOTICE([libjderobotice not enabled: gearbox not found])
fi
