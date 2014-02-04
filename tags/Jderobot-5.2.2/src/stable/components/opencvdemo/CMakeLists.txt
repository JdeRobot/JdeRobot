SET( SOURCE_FILES  opencvdemo.cpp viewer.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
)


add_executable (opencvdemo ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(opencvdemo
    ${CMAKE_THREAD_LIBS_INIT} 
    ${OpenCV_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
    jderobotutil
#    ${Gearbox_LIBRARIES}
    ${ZeroCIce_LIBRARIES}


)
