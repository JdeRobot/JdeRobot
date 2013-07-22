IF(with_pcl)

    SET( SOURCE_FILES  recorder.cpp recordergui.cpp)

      set( CMAKE_CXX_FLAGS "-Wno-deprecated -lGL -lGLU -lglut -lm -lusb-1.0 " ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo

    include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
	  /usr/include/libusb-1.0
	  /usr/include/cminpack-1 
	  /usr/include/eigen3 
	  /usr/include/vtk-5.8/ 
    )



    add_executable (recorder ${SOURCE_FILES})


    TARGET_LINK_LIBRARIES(recorder
          ${CMAKE_THREAD_LIBS_INIT} 
	  ${opencv_LIBRARIES}
	  ${gtkmm_LIBRARIES}
	  ${libglademm_LIBRARIES}

	  ${gtkglextmm_LIBRARIES}
	  ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
	  ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
	  ${LIBS_DIR}/jderobotutil/libjderobotutil.so
	  ${LIBS_DIR}/progeo/libprogeo.so
	  ${LIBS_DIR}/pioneer/libpioneer.so
	${ZeroCIce_LIBRARIES}
	${gsl_LIBRARIES}

   
    )
ENDIF()
