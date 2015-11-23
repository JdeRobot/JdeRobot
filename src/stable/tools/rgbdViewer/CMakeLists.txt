SET(SOURCE_FILES rgbdViewer.cpp drawarea.cpp rgbdViewergui.cpp myprogeo.cpp)

add_definitions(-DGLADE_DIR="${gladedir}")

set( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${gtkglextmm_INCLUDE_DIRS}
)

add_executable (rgbdViewer ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(rgbdViewer
    ${CMAKE_THREAD_LIBS_INIT} 
    ${GLUT_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
    jderobotutil
    progeo
	geometry
    pioneer
    parallelIce
    ${gsl_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    ${ZLIB_LIBRARIES}
)
