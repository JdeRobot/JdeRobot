SET( SOURCE_FILES cameraview.cpp viewer.cpp)

include(FindPkgConfig)
PKG_CHECK_MODULES(gstreamer REQUIRED gstreamer-0.10) # REQUIRED=Fallará si no lo encuentra
PKG_CHECK_MODULES(gstreamerapp REQUIRED gstreamer-app-0.10) # REQUIRED=Fallará si no lo encuentra
add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gstreamer_INCLUDE_DIRS}
    ${gstreamerapp_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
)

link_directories(
    ${gstreamer_LIBRARY_DIRS}
    ${gstreamerapp_LIBRARY_DIRS}
)

add_executable (cameraview ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(cameraview 
    ${gstreamerapp_LIBRARIES}
    ${gstreamer_LIBRARIES}
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
)