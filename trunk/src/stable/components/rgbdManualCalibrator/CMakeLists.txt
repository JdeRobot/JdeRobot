SET( SOURCE_FILES common.h controller.cpp drawarea.cpp view.h rgbdManualCalibrator.cpp controller.h drawarea.h view.cpp)
add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${gl_INCLUDE_DIRS}
    ${glu_INCLUDE_DIRS}
    ${glut_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR})

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
 ${LIBS_DIR}/jderobotutil/libjderobotutil.so
 ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
 ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
 ${LIBS_DIR}/progeo/libprogeo.so
 ${LIBS_DIR}/parallelIce/libparallelIce.so
    #-lxerces-c
)
