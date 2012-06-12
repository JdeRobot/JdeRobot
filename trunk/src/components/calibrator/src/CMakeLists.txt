SET( SOURCE_FILES calibrator.cpp view.cpp view.h module_dlt.cpp module_dlt.h module_extrinsics.cpp module_extrinsics.h module_rectifier.cpp module_rectifier.h drawarea.cpp drawarea.h)
add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${gl_INCLUDE_DIRS}
    ${glu_INCLUDE_DIRS}
    ${glut_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR})

set( CMAKE_CXX_FLAGS "-lglut")
add_executable (calibrator ${SOURCE_FILES})

TARGET_LINK_LIBRARIES( calibrator
 ${opencv_LIBRARIES}
 ${gtkmm_LIBRARIES}
 ${libglademm_LIBRARIES}
 ${gthread_LIBRARIES}
 ${libgnomecanvas_LIBRARIES}
 ${libgnomecanvasmm_LIBRARIES}
 ${gtkglextmm_LIBRARIES}
 ${gsl_LIBRARIES}
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/progeo/libprogeo.so
-lxerces-c
)
