
SET( SOURCE_FILES API.cpp camera.h camera.cpp control.cpp drawarea.cpp gui.cpp introrob.cpp MyAlgorithms.cpp pioneer.c pioneeropengl.c)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
    ${CMAKE_CURRENT_SOURCE_DIR}/../calibrator
)

set( CMAKE_CXX_FLAGS "-lglut -lgsl -lgslcblas -lIce -lIceUtil" ) # Opciones para el compilador

add_executable (introrob  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(introrob 
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/progeo/libprogeo.so
    ${LIBS_DIR}/pioneer/libpioneer.so
)

