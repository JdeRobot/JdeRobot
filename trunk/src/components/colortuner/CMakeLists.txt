SET( SOURCE_FILES viewer.cpp colorTuner.cpp)

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${INTERFACES_CPP_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
)

add_executable (colortuner ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(colortuner 
    ${opencv_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspaces.so
    ${Gearbox_LIBRARIES}
    ${ZeroCIce_LIBRARIES}

)

