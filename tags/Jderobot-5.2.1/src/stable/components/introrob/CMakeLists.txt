
SET( SOURCE_FILES API.cpp  control.cpp drawarea.cpp gui.cpp introrob.cpp MyAlgorithms.cpp pioneer.c pioneeropengl.c)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
    ${CMAKE_CURRENT_SOURCE_DIR}/../calibrator
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${gtkglextmm_INCLUDE_DIRS}
    ${libgnomecanvas_INCLUDE_DIRS}
    ${libgnomecanvasmm_INCLUDE_DIRS}
)

set( CMAKE_CXX_FLAGS "") #"-lgsl -lgslcblas" ) # Opciones para el compilador

add_executable (introrob  ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(introrob
    ${CMAKE_THREAD_LIBS_INIT} 
    ${opencv_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${OpenCV_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
    jderobotutil
    progeo
    pioneer
    "glut"
    "gsl"
    "gslcblas"
#	${gsl_LIBRARIES}
	${ZeroCIce_LIBRARIES}
	${Gearbox_LIBRARIES}
)
