SET( SOURCE_FILES calibrator.cpp view.cpp view.h module_dlt.cpp module_dlt.h module_extrinsics.cpp module_extrinsics.h module_rectifier.cpp module_rectifier.h drawarea.cpp drawarea.h)
add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})


add_executable (calibrator ${SOURCE_FILES})

TARGET_LINK_LIBRARIES( calibrator
 ${CMAKE_THREAD_LIBS_INIT}
 ${gtkmm_LIBRARIES}
 ${libglademm_LIBRARIES}
  ${GLUT_LIBRARIES}
 ${gtkglextmm_LIBRARIES}
 ${ZeroCIce_LIBRARIES}
 ${OpenCV_LIBRARIES}
 ${gsl_LIBRARIES}
 ${LIBS_DIR}/jderobotutil/libjderobotutil.so
 ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
 ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
 ${LIBS_DIR}/progeo/libprogeo.so
)
