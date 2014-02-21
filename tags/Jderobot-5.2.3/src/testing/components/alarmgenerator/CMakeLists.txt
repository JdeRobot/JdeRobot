#SET (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}
#	CACHE PATH "Directory for Libraries")
#SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}
#	CACHE PATH "Directory for Executables")
set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo" ) # Opciones para el compilador
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cameras)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/cpp/jderobot)


add_executable (alarmgenerator alarmgenerator.cpp )

TARGET_LINK_LIBRARIES(alarmgenerator ${opencv_LIBRARIES})
TARGET_LINK_LIBRARIES(alarmgenerator ${gtkmm_LIBRARIES})
TARGET_LINK_LIBRARIES(alarmgenerator ${libglademm_LIBRARIES})
TARGET_LINK_LIBRARIES(alarmgenerator ${gthread_LIBRARIES})
TARGET_LINK_LIBRARIES(alarmgenerator ${libgnomecanvas_LIBRARIES})
TARGET_LINK_LIBRARIES(alarmgenerator ${libgnomecanvasmm_LIBRARIES})
TARGET_LINK_LIBRARIES(alarmgenerator ${gtkglextmm_LIBRARIES})



#TARGET_LINK_LIBRARIES(alarmgenerator ${PROJECT_BINARY_DIR}/src/interfaces/cpp/jderobot/libJderobotInterfaces.so)
TARGET_LINK_LIBRARIES(alarmgenerator /usr/local/lib/jderobot/libJderobotInterfaces.so)

