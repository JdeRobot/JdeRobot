 cmake_minimum_required(VERSION 2.8)


SET( SOURCE_FILES replayController.cpp replayControllerGui.cpp)


add_definitions(-DGLADE_DIR="${gladedir}")

set( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable (replayController ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(replayController 
	${GLUT_LIBRARIES}
   	${OpenCV_LIBRARIES}
   	${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/progeo/libprogeo.so
    ${LIBS_DIR}/pioneer/libpioneer.so
    ${LIBS_DIR}/parallelIce/libparallelIce.so
    ${ZeroCIce_LIBRARIES}
)
