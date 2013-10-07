SET( SOURCE_FILES common.h controller.cpp drawarea.cpp view.h rgbdManualCalibrator.cpp controller.h drawarea.h view.cpp)
add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${gl_INCLUDE_DIRS}
    ${glu_INCLUDE_DIRS}
    ${glut_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${gtkglextmm_INCLUDE_DIRS}
    )

add_executable (rgbdManualCalibrator ${SOURCE_FILES})

TARGET_LINK_LIBRARIES( rgbdManualCalibrator
 ${GLUT_LIBRARIES}
 ${opencv_LIBRARIES}
 ${OpenCVGUI_LIBRARIES}
 ${gtkmm_LIBRARIES}
 ${libglademm_LIBRARIES}
 ${gthread_LIBRARIES}
 ${libgnomecanvas_LIBRARIES}
 ${libgnomecanvasmm_LIBRARIES}
 ${gtkglextmm_LIBRARIES}
 ${ZeroCIce_LIBRARIES}
 ${OpenCV_LIBRARIES}
 ${gsl_LIBRARIES}
 ${CMAKE_THREAD_LIBS_INIT}
 jderobotutil
 colorspacesmm
 JderobotInterfaces
 progeo
 parallelIce
    #-lxerces-c
)
