SET( SOURCE_FILES  opencvdemo.cpp viewer.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable (opencvdemo ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(opencvdemo
    ${CMAKE_THREAD_LIBS_INIT} 
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
#    ${gthread_LIBRARIES}
#    ${libgnomecanvas_LIBRARIES}
#    ${libgnomecanvasmm_LIBRARIES}
#    ${gtkglextmm_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${Gearbox_LIBRARIES}
    ${ZeroCIce_LIBRARIES}

)
