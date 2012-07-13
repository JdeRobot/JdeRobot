IF(with_pcl)



set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo -lpcl_features -lpcl_kdtree -lpcl_common -lpcl_visualization -lpcl_surface -lpcl_registration -lpcl_segmentation -lpcl_filters -lpcl_sample_consensus -lpcl_io" ) # Opciones para el compilador

SET( SOURCE_FILES replayer.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
	/usr/include/pcl-1.5/
	/usr/include/eigen3/
)

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
)
endif()
