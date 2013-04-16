IF(openni_LIBRARIES)
    IF(nite_LIBRARIES)
	    SET( SOURCE_FILES openniServer-kinect.cpp myprogeo.cpp )
	    add_definitions(-DGLADE_DIR="${gladedir}")

	    set( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo

	    include_directories(
            ${INTERFACES_CPP_DIR}
            ${LIBS_DIR}/
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/cameras
	        ${nite_INCLUDE_DIR}
	        ${ni_INCLUDE_DIR}
        )

	    add_executable (openniServer-kinect ${SOURCE_FILES})

        TARGET_LINK_LIBRARIES(openniServer-kinect
            ${LIBS_DIR}/jderobotice/libjderobotice.so
            ${LIBS_DIR}/jderobotutil/libjderobotutil.so
            ${LIBS_DIR}/progeo/libprogeo.so
            ${LIBS_DIR}/pioneer/libpioneer.so
            ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
            ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
            ${Gearbox_LIBRARIES}
	        ${ZeroCIce_LIBRARIES}
	        ${openni_LIBRARIES}
			${nite_LIBRARIES}
	        ${pcl_LIBRARIES}
            ${opencv_LIBRARIES}
            ${gsl_LIBRARIES}
#            ${gthread_LIBRARIES}
        )
    ENDIF()
ENDIF()
