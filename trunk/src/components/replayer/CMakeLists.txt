IF(with_pcl)


SET( SOURCE_FILES replayer.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
	${pcl_INCLUDE_DIR}/
	/usr/include/eigen3/
)

set( CMAKE_CXX_FLAGS "-lIce -lIceUtil" ) # Opciones para el compilador

add_executable (replayer ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(replayer
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
	${Gearbox_LIBRARIES}
	${ZeroCIce_LIBRARIES}
	${pcl_LIBRARIES}
	${gsl_LIBRARIES}
)
endif()
