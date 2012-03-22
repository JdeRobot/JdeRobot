SET( SOURCE_FILES cvfast.cpp geometry.cpp image.cpp linesDetection.cpp)

set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas" ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo

include_directories(
    ${SLICE_DIR}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library (visionlib  SHARED ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(visionlib 
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/progeo/libprogeo.so
)
