SET( SOURCE_FILES cameraview.cpp viewer.cpp)

add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable (cameraview ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(cameraview 
    ${OpenCV_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
   ${ZeroCIce_LIBRARIES}
)
