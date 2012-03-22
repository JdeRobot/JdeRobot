IF(with_fireware)
    SET( SOURCE_FILES cameraserver.cpp gstpipeline.cpp)

    include(FindPkgConfig)
    PKG_CHECK_MODULES(gstreamer REQUIRED gstreamer-0.10) # REQUIRED=Fallará si no lo encuentra
    PKG_CHECK_MODULES(gstreamerapp REQUIRED gstreamer-app-0.10) # REQUIRED=Fallará si no lo encuentra
    add_definitions(-DGLADE_DIR="${gladedir}")

      set( CMAKE_CXX_FLAGS " -lraw1394 -ldc1394")


    include_directories(
	${INTERFACES_CPP_DIR}
	${LIBS_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${gstreamer_INCLUDE_DIRS}
	${gstreamerapp_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/cameras
	${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/cpp/jderobot
	${with_fireware_LIBRARY_DIRS}}
    )

    link_directories(
	${gstreamer_LIBRARY_DIRS}
	${gstreamerapp_LIBRARY_DIRS}
	${with_fireware_INCLUDE_DIRS}

    )

    add_executable (cameraserver ${SOURCE_FILES})

    TARGET_LINK_LIBRARIES(cameraserver 
	${gstreamerapp_LIBRARIES}
	${gstreamer_LIBRARIES}
	${opencv_LIBRARIES}
	${gtkmm_LIBRARIES}
	${libglademm_LIBRARIES}
	${gthread_LIBRARIES}
	${libgnomecanvas_LIBRARIES}
	${libgnomecanvasmm_LIBRARIES}
	${gtkglextmm_LIBRARIES}
	${LIBS_DIR}/jderobotice/libjderobotice.so
	${LIBS_DIR}/jderobotutil/libjderobotutil.so
	${LIBS_DIR}/colorspaces/libcolorspacesmm.so
	${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    )
ENDIF()