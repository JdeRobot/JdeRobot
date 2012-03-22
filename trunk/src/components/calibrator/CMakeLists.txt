SET( SOURCE_FILES calibrator.cpp controller.cpp view.cpp )

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (calibrator ${SOURCE_FILES})

TARGET_LINK_LIBRARIES( calibrator
 ${opencv_LIBRARIES}
 ${gtkmm_LIBRARIES}
 ${libglademm_LIBRARIES}
 ${gthread_LIBRARIES}
 ${libgnomecanvas_LIBRARIES}
 ${libgnomecanvasmm_LIBRARIES}
 ${gtkglextmm_LIBRARIES}
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/progeo/libprogeo.so
)