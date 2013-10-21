set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo" ) # Opciones para el compilador
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/cpp/jderobot)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/cameras)

add_executable (naobody API.cpp API.h camera.h camera.cpp control.cpp control.h drawarea.cpp drawarea.h gui.cpp gui.h naobody.cpp MyAlgorithms.cpp pioneer.c pioneer.h pioneeropengl.h pioneeropengl.c)


TARGET_LINK_LIBRARIES(naobody ${opencv_LIBRARIES})
TARGET_LINK_LIBRARIES(naobody ${gtkmm_LIBRARIES})
TARGET_LINK_LIBRARIES(naobody ${libglademm_LIBRARIES})
TARGET_LINK_LIBRARIES(naobody ${gthread_LIBRARIES})
TARGET_LINK_LIBRARIES(naobody ${libgnomecanvas_LIBRARIES})
TARGET_LINK_LIBRARIES(naobody ${libgnomecanvasmm_LIBRARIES})
TARGET_LINK_LIBRARIES(naobody ${gtkglextmm_LIBRARIES})
TARGET_LINK_LIBRARIES(naobody ${LIBS_DIR}/jderobotice/libjderobotice.so) # Modificar por ruta donde se alojarán las librerías de jderobot propias
TARGET_LINK_LIBRARIES(naobody ${LIBS_DIR}/jderobotutil/libjderobotutil.so)
TARGET_LINK_LIBRARIES(naobody ${LIBS_DIR}/colorspaces/libcolorspacesmm.so)
TARGET_LINK_LIBRARIES(naobody ${PROJECT_BINARY_DIR}/src/interfaces/cpp/jderobot/libJderobotInterfaces.so)
TARGET_LINK_LIBRARIES(naobody ${LIBS_DIR}/progeo/libprogeo.so)
TARGET_LINK_LIBRARIES(naobody ${LIBS_DIR}/pioneer/libpioneer.so)


