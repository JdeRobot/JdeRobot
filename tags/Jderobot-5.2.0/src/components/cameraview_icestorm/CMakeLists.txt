SET( SOURCE_FILES viewer.cpp cameraview_icestorm.cpp)
add_definitions(-DGLADE_DIR="${gladedir}")
include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${INTERFACES_CPP_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
)

add_executable (cameraview_icestorm ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(cameraview_icestorm
    ${CMAKE_THREAD_LIBS_INIT} 
    ${OpenCV_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
#    ${gthread_LIBRARIES}
#    ${libgnomecanvas_LIBRARIES}
#    ${libgnomecanvasmm_LIBRARIES}
#    ${gtkglextmm_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${ZeroCIceCore_LIBRARY}
    ${ZeroCIceStorm_LIBRARY}
    ${ZeroCIce_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${Gearbox_LIBRARIES}
)
