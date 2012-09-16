SET( gladedir ${CMAKE_CURRENT_SOURCE_DIR})

SET( SOURCE_FILES controller.cpp main.cpp mainthread.cpp model.cpp viewgtk.cpp viewtext.cpp )

set( CMAKE_CXX_FLAGS "-lglut -lgsl -lgslcblas " ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo
add_definitions(-DGLADE_DIR="${gladedir}")


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (bgfglab ${SOURCE_FILES})

TARGET_LINK_LIBRARIES( bgfglab
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
    ${LIBS_DIR}/bgfgsegmentation/libbgfgsegmentation.so
    ${Gearbox_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    )
