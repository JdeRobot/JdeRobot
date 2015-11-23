SET( SOURCE_FILES giraffeClient.cpp  controller.cpp controller.h view.cpp cameraconf.cpp )

add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
)



add_executable (giraffeClient ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(giraffeClient
    ${CMAKE_THREAD_LIBS_INIT}  
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gsl_LIBRARIES}
    ${OpenCV_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
    jderobotutil
    progeo
    ${ZeroCIce_LIBRARIES}
)

