SET( SOURCE_FILES rgbdCalibrator.cpp viewer.cpp calibration.cpp geoUtils.cpp)

add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable (rgbdCalibrator ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(rgbdCalibrator
    ${CMAKE_THREAD_LIBS_INIT}
    ${alut_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${ZeroCIce_LIBRARIES}
    ${Boost_LIBRARIES}
    ${LIBS_DIR}/progeo/libprogeo.so
    ${LIBS_DIR}/geometry/libgeometry.so
    ${LIBS_DIR}/colorspaces/libcolorspaces.so
    ${LIBS_DIR}/cvBlob/libcvblob.so
    ${gsl_LIBRARIES}
)
