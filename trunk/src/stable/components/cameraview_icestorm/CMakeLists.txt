SET( SOURCE_FILES viewer.cpp cameraview_icestorm.cpp)
add_definitions(-DGLADE_DIR="${gladedir}")
include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${gtkglextmm_INCLUDE_DIRS}
)

add_executable (cameraview_icestorm ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(cameraview_icestorm
    ${CMAKE_THREAD_LIBS_INIT} 
    ${OpenCV_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
   ${ZeroCIce_LIBRARIES}
)
