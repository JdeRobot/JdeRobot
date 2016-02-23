SET(SOURCE_FILES recordergui.cpp poolWriteImages.cpp poolWritePose3dEncoders.cpp poolWriteLasers.cpp poolWritePointCloud.cpp poolWriteEncoders.cpp )

add_definitions(-DGLADE_DIR="${gladedir}")

set( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${gtkglextmm_INCLUDE_DIRS}
)

add_executable (recorder2
recorder2.cpp
poolWritePose3d.cpp
${SOURCE_FILES})


TARGET_LINK_LIBRARIES(recorder2
    ${CMAKE_THREAD_LIBS_INIT} 
    ${GLUT_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${gsl_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    ${Boost_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
    jderobotutil
)
