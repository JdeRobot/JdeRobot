SET( SOURCE_FILES cameraCalibrator.cpp )


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
)



add_executable (cameraCalibrator  ${SOURCE_FILES})

    TARGET_LINK_LIBRARIES(cameraCalibrator
        ${OpenCV_LIBRARIES}
        ${OpenCVGUI_LIBRARIES}
        ${gtkmm_LIBRARIES}
        ${libglademm_LIBRARIES}
        colorspacesmm
        JderobotInterfaces
        jderobotutil
        parallelIce
        ${ZLIB_LIBRARIES}
        #    ${Gearbox_LIBRARIES}
        ${ZeroCIce_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT} 
)
