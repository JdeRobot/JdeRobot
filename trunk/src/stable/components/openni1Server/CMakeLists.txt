IF(openni_LIBRARIES)

	    SET( SOURCE_FILES openni1Server.cpp myprogeo.cpp )
	    add_definitions(-DGLADE_DIR="${gladedir}")

	    set( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador

	    include_directories(
            ${INTERFACES_CPP_DIR}
            ${LIBS_DIR}/
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/cameras
            ${openni_INCLUDE_DIR}
            ${LIBUSB_INCLUDE_DIRS}
        )

	    add_executable (openni1Server ${SOURCE_FILES})



			IF( nite_LIBRARIES)

				set_property(
			  		SOURCE openni1Server.cpp
			   		PROPERTY COMPILE_DEFINITIONS WITH_NITE=1
			   	)
				include_directories(
				    ${nite_INCLUDE_DIR}
		    	)

			TARGET_LINK_LIBRARIES(openni1Server
				  ${OpenCV_LIBRARIES}
				  ${OpenCVGUI_LIBRARIES}
				  ${gsl_LIBRARIES}
				  ${ZeroCIce_LIBRARIES} 
				  ${openni_LIBRARIES}
				  ${nite_LIBRARIES}
				  colorspacesmm
				  JderobotInterfaces
				  jderobotutil
				  progeo
				)
		ELSE()
			TARGET_LINK_LIBRARIES(openni1Server
				  ${OpenCV_LIBRARIES}
				  ${OpenCVGUI_LIBRARIES}
				  ${gsl_LIBRARIES}
				  ${ZeroCIce_LIBRARIES} 
				  ${openni_LIBRARIES}
				  colorspacesmm
				  colorspacesmm
				  JderobotInterfaces
				  jderobotutil
				  progeo
				)
		ENDIF()
ENDIF()
