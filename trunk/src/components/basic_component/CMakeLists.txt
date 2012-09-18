   

set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas" ) # Opciones para el compilador -lgslcblas -lGL -lGLU -lglut -lgazebo

SET( SOURCE_FILES  control.cpp camera.cpp basic_component.cpp API.cpp gui.cpp canvasTeleoperateCameras.cpp canvasTeleoperateControl.cpp canvasLaser.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
    ${INCLUDE_DIR}
)

link_directories(
   ${INTERFACES_CPP_DIR}
   ${LIBS_DIR}
)



add_executable (basic_component ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(basic_component 
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/progeo/libprogeo.so
    ${LIBS_DIR}/pioneer/libpioneer.so
)




