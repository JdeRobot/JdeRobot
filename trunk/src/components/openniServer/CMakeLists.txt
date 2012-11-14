IF(with_openni)
    IF(with_nite)
	SET( SOURCE_FILES openniServer.cpp myprogeo.cpp )
	add_definitions(-DGLADE_DIR="${gladedir}")

	set( CMAKE_CXX_FLAGS "-Wno-deprecated -lgsl -lgslcblas" ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo


	include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
	${nite_INCLUDE_DIR}
	${ni_INCLUDE_DIR}
)


	add_executable (openniServer ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(openniServer
    	${opencv_LIBRARIES}
    ${gthread_LIBRARIES}
    ${Gearbox_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/progeo/libprogeo.so
    ${LIBS_DIR}/pioneer/libpioneer.so
	${ZeroCIce_LIBRARIES}
	${pcl_LIBRARIES}
	${gsl_LIBRARIES}
	${OpenNi_LIBRARY}
)
    ENDIF()
ENDIF()
