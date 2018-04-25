SET( SOURCE_FILES rgbdCalibrator.cpp viewer.cpp calibration.cpp geoUtils.cpp)

add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${resourcelocator_INCLUDE_DIRS}
    ${easyiceconfig_INCLUDE_DIRS}
)

link_directories(
	${resourcelocator_LIBRARY_DIRS}
	${easyiceconfig_LIBRARY_DIRS}
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
    ${easyiceconfig_LIBRARIES} 
    ${ZeroCIce_LIBRARIES}
    ${Boost_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
    geometry
    colorspaces
    cvblob
	${resourcelocator_LIBRARIES}
	${GLOG_LIBRARIES}
	)

install(TARGETS rgbdCalibrator
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
    COMPONENT rgbdCalibrator
    )

INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/rgbdCalibrator.glade DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/glade COMPONENT rgbdCalibrator)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/rgbdCalibrator.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT rgbdCalibrator)
