SET( SOURCE_FILES  naooperator.cpp naooperatorgui.cpp controllers/bodyencoders-controller.cpp controllers/bodymotors-controller.cpp controllers/motors-controller.cpp controllers/ptencoders-controller.cpp controllers/ptmotors-controller.cpp gui-modules/bodyencodersGui.cpp gui-modules/bodymotorsGui.cpp gui-modules/motorsGui.cpp gui-modules/ptencodersGui.cpp gui-modules/ptmotorsGui.cpp)
add_definitions(-DGLADE_DIR="${gladedir}")


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
)


add_executable (naooperator ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(naooperator 
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
#    ${gthread_LIBRARIES}
#    ${libgnomecanvas_LIBRARIES}
#    ${libgnomecanvasmm_LIBRARIES}
#    ${gtkglextmm_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${LIBS_DIR}/colorspaces/libcolorspaces.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
#    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${Gearbox_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    ${OpenCV_LIBRARIES}
)
