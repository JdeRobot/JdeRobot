IF(with_openni)
    IF(with_nite)
	SET( SOURCE_FILES openniServer.cpp myprogeo.cpp )
	add_definitions(-DGLADE_DIR="${gladedir}")

	set( CMAKE_CXX_FLAGS "-Wno-deprecated -lgsl -lgslcblas -lOpenNI" ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo


	include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
	/usr/include/ni/
	/usr/include/nite/
)


	add_executable (openniServer ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(openniServer
    	${opencv_LIBRARIES}
    	${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${Gearbox_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/progeo/libprogeo.so
    ${LIBS_DIR}/pioneer/libpioneer.so
)
    ENDIF()
ENDIF()
