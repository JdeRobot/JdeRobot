SET( SOURCE_FILES viewer.cpp colorTuner.cpp)

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${easyiceconfig_INCLUDE_DIRS}
)

link_directories(
	${easyiceconfig_LIBRARY_DIRS}
)

add_executable (colorTuner ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(colorTuner 
    ${CMAKE_THREAD_LIBS_INIT}
    ${opencv_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${gtkmm_LIBRARIES}
    colorspacesmm
    colorspaces
    JderobotInterfaces
    jderobotutil
    ${Gearbox_LIBRARIES}
    ${easyiceconfig_LIBRARIES} 
    ${ZeroCIce_LIBRARIES}

)

