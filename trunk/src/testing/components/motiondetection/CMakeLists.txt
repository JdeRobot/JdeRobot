SET( SOURCE_FILES_LIB motiondetection.cpp view.cpp model.cpp controller.cpp)

include_directories(
    ${SLICE_DIR}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(motiondetectionshare SHARED ${SOURCE_FILES_LIB})

TARGET_LINK_LIBRARIES( motiondetectionshare
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
)


set_target_properties(motiondetectionshare PROPERTIES OUTPUT_NAME motiondetection)


#link_directories(/usr/local/lib)
include_directories(
    ${SLICE_DIR}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

SET( SOURCE_FILES_EXE main.cpp)
set( CMAKE_CXX_FLAGS -DGLADE_DIR=\""$(gladedir)"\")


add_executable (motiondetectionexe ${SOURCE_FILES_EXE})



TARGET_LINK_LIBRARIES(motiondetectionexe 
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
   ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${CMAKE_CURRENT_SOURCE_DIR}/libmotiondetection.so
   
)
