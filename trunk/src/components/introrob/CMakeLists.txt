SET( SOURCE_FILES API.cpp camera.h camera.cpp control.cpp drawarea.cpp gui.cpp introrob.cpp MyAlgorithms.cpp pioneer.c pioneeropengl.c)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
)

set( CMAKE_CXX_FLAGS "-lglut -lgsl -lgslcblas" ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo

add_executable (introrob  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(introrob 
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/progeo/libprogeo.so
    ${LIBS_DIR}/pioneer/libpioneer.so
)

