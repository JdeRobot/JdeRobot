IF(openni_LIBRARIES)
    IF(nite_LIBRARIES)
	    SET( SOURCE_FILES openni1Server.cpp myprogeo.cpp )
	    add_definitions(-DGLADE_DIR="${gladedir}")

	    set( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador

	    include_directories(
            ${INTERFACES_CPP_DIR}
            ${LIBS_DIR}/
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/cameras
	        ${nite_INCLUDE_DIR}
	        ${ni_INCLUDE_DIR}
        )

	    add_executable (openni1Server ${SOURCE_FILES})

        FIND_PACKAGE(libusb-1.0 REQUIRED)
        MESSAGE(STATUS ${usb_LIBRARIES})
        TARGET_LINK_LIBRARIES(openni1Server
            progeo
            colorspacesmm
            JderobotInterfaces
            ${ZeroCIce_LIBRARIES}
            ${openni_LIBRARIES}
            ${nite_LIBRARIES}
            ${opencv_LIBRARIES}
            ${gsl_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
        )
    ENDIF()
ENDIF()
