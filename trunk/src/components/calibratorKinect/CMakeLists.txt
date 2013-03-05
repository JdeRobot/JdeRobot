SET( SOURCE_FILES common.h controller.cpp drawarea.cpp view.h calibratorKinect.cpp controller.h drawarea.h view.cpp)
add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${gl_INCLUDE_DIRS}
    ${glu_INCLUDE_DIRS}
    ${glut_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (calibratorKinect ${SOURCE_FILES})

TARGET_LINK_LIBRARIES( calibratorKinect
 ${GLUT_LIBRARIES}
 ${opencv_LIBRARIES}
 ${gtkmm_LIBRARIES}
 ${libglademm_LIBRARIES}
 ${gthread_LIBRARIES}
 ${libgnomecanvas_LIBRARIES}
 ${libgnomecanvasmm_LIBRARIES}
 ${gtkglextmm_LIBRARIES}
 ${ZeroCIce_LIBRARIES}
 ${OpenCV_LIBRARIES}
 ${gsl_LIBRARIES}
 ${LIBS_DIR}/jderobotice/libjderobotice.so
 ${LIBS_DIR}/jderobotutil/libjderobotutil.so
 ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
 ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
 ${LIBS_DIR}/progeo/libprogeo.so
    #-lxerces-c
)
