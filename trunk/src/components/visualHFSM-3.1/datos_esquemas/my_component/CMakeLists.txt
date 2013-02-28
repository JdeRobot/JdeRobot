   

set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas" ) # Opciones para el compilador -lgslcblas -lGL -lGLU -lglut -lgazebo

SET( SOURCE_FILES camera.cpp canvasLaser.cpp canvas_main.cpp canvasTeleoperateCameras.cpp canvasTeleoperateControl.cpp control_class.cpp gui.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
    ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/cpp/jderobot
    ${INCLUDE_DIR}
)

link_directories(
   ${INTERFACES_CPP_DIR}
   ${LIBS_DIR}
)



add_executable (mycomponent ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(choca_gira 
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




