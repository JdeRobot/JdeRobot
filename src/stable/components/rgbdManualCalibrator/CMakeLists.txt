SET( SOURCE_FILES common.h controller.cpp drawarea.cpp view.h rgbdManualCalibrator.cpp controller.h drawarea.h view.cpp)
add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${gl_INCLUDE_DIRS}
    ${glu_INCLUDE_DIRS}
    ${glut_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${gtkglextmm_INCLUDE_DIRS}
    )
find_package( OpenCV REQUIRED )
add_executable (rgbdManualCalibrator ${SOURCE_FILES})

TARGET_LINK_LIBRARIES( ${OpenCV_CFLAGS} 
rgbdManualCalibrator
 ${OpenCV_LIBS}	
 ${GLUT_LIBRARIES}
 
 ${opencv_LIBRARIES}
 ${OpenCVGUI_LIBRARIES}
 ${gtkmm_LIBRARIES}
 ${libglademm_LIBRARIES}
 ${gthread_LIBRARIES}
 ${libgnomecanvas_LIBRARIES}
 ${libgnomecanvasmm_LIBRARIES}
 ${gtkglextmm_LIBRARIES}

 ${ZeroCIce_LIBRARIES}
 ${OpenCV_LIBRARIES}
${OpenCVGUI_LIBRARIES}
 ${gsl_LIBRARIES}
 ${CMAKE_THREAD_LIBS_INIT}
 ${OpenCV_CFLAGS} 
jderobotutil
 ${OpenCV_LIBS}
 ${OpenCV_CFLAGS} 
colorspacesmm
 ${OpenCV_LIBS}
 ${OpenCV_CFLAGS} 
JderobotInterfaces
 ${OpenCV_LIBS}
 ${OpenCV_CFLAGS} 
progeo
 ${OpenCV_LIBS}
 ${OpenCV_CFLAGS} 
parallelIce
 ${OpenCV_LIBS}
 ${Boost_LIBRARIES} ${ZLIB_LIBRARIES}
    #-lxerces-c
)
