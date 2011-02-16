dnl # Requirements for libbgfgsegmentation
dnl # OpenCV


AC_MSG_NOTICE([**** Checking libbgfgsegmentation requirements:])
if test "x$with_opencv" = xno; then
    AM_CONDITIONAL([ENABLE_LIBBGFGSEGMENTATION],[false])
    with_bgfgsegmentation="no"
    AC_MSG_NOTICE([libbgfgsegmentation not enabled: Ice not found])
else
    AM_CONDITIONAL([ENABLE_LIBBGFGSEGMENTATION],[true])
    with_bgfgsegmentation="yes"
    AC_SUBST([BGFGSEGMENTATION_CPPFLAGS],["$OPENCV_CPPFLAGS -I\${top_srcdir}/src/libs"])
    AC_SUBST([BGFGSEGMENTATION_LDFLAGS],["$OPENCV_LDFLAGS"])
    AC_MSG_NOTICE([libbgfgsegmentation enabled])
fi
