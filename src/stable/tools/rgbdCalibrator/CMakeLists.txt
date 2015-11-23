SET( SOURCE_FILES rgbdCalibrator.cpp viewer.cpp calibration.cpp geoUtils.cpp)

add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
)

add_executable (rgbdCalibrator ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(rgbdCalibrator
    ${CMAKE_THREAD_LIBS_INIT}
    ${gsl_LIBRARIES}
    ${alut_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    ${Boost_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
    progeo
    geometry
    colorspaces
    cvblob
    
)
