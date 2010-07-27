dnl # Requirements for libbgfgfilters
dnl # OpenCV


AC_MSG_NOTICE([**** Checking libbgfgfilters requirements:])
if test "x$with_opencv" = xno; then
    AM_CONDITIONAL([ENABLE_LIBBGFGFILTERS],[false])
    with_bgfgfilters="no"
    AC_MSG_NOTICE([libbgfgfilters not enabled: Ice not found])
else
    AM_CONDITIONAL([ENABLE_LIBBGFGFILTERS],[true])
    with_bgfgfilters="yes"
    AC_SUBST([BGFGFILTERS_CPPFLAGS],["$OPENCV_CPPFLAGS" -I\${top_srcdir}/src/libs])
    AC_SUBST([BGFGFILTERS_LDFLAGS],["$OPENCV_LDFLAGS"])
    AC_MSG_NOTICE([libbgfgfilters enabled])
fi