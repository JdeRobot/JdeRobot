set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas -lglut -lOpenNI -g -O -malign-double -O3 -fno-tree-pre -fno-strict-aliasing -DNDEBUG -msse3 -mssse3 -DUSE_GLUT -DXN_SSE" ) # Opciones para el compilador
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(
  /usr/include/gearbox/
  /usr/include/nite
  /usr/include/openni
  ../Bin
  /usr/include/ni/
    ${SLICE_DIR}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable (openniServer openniServer.cpp)


TARGET_LINK_LIBRARIES(openniServer ${opencv_LIBRARIES})
TARGET_LINK_LIBRARIES(openniServer ${gtkmm_LIBRARIES})
TARGET_LINK_LIBRARIES(openniServer ${libglademm_LIBRARIES})
TARGET_LINK_LIBRARIES(openniServer ${gthread_LIBRARIES})
TARGET_LINK_LIBRARIES(openniServer ${libgnomecanvas_LIBRARIES})
TARGET_LINK_LIBRARIES(openniServer ${libgnomecanvasmm_LIBRARIES})
TARGET_LINK_LIBRARIES(openniServer ${gtkglextmm_LIBRARIES})
TARGET_LINK_LIBRARIES(openniServer ${LIBS_DIR}/jderobotice/libjderobotice.so) # Modificar por ruta donde se alojarán las librerías de jderobot propias
TARGET_LINK_LIBRARIES(openniServer ${LIBS_DIR}/jderobotutil/libjderobotutil.so)
TARGET_LINK_LIBRARIES(openniServer ${LIBS_DIR}/colorspaces/libcolorspacesmm.so)
TARGET_LINK_LIBRARIES(openniServer ${PROJECT_BINARY_DIR}/src/interfaces/cpp/jderobot/libJderobotInterfaces.so)
TARGET_LINK_LIBRARIES(openniServer ${LIBS_DIR}/progeo/libprogeo.so)