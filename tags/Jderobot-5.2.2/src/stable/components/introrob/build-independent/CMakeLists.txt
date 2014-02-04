cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)


SET( SOURCE_FILES ../API.cpp  ../control.cpp ../drawarea.cpp ../gui.cpp ../introrob.cpp ../MyAlgorithms.cpp ../pioneer.c ../pioneeropengl.c)
SET(INTERFACES_CPP_DIR /usr/local/lib )


include_directories(
    ${INTERFACES_CPP_DIR}
    /usr/local/include/jderobot/
	/usr/local/include/jderobot/progeo/
)

set( CMAKE_CXX_FLAGS "-lglut -lgsl -lIce -lIceUtil -lgslcblas" ) # Opciones para el compilador

PKG_CHECK_MODULES(gtkmm REQUIRED gtkmm-2.4)
include_directories(${gtkmm_INCLUDE_DIRS})
link_directories(${gtkmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gthread REQUIRED gthread-2.0)
include_directories(${gthread_INCLUDE_DIRS})
link_directories(${gthread_LIBRARY_DIRS})

PKG_CHECK_MODULES(libglademm REQUIRED libglademm-2.4)
include_directories(${libglademm_INCLUDE_DIRS})
link_directories(${libglademm_LIBRARY_DIRS})

PKG_CHECK_MODULES(libgnomecanvas REQUIRED libgnomecanvas-2.0)
include_directories(${libgnomecanvas_INCLUDE_DIRS})
link_directories(${libgnomecanvas_LIBRARY_DIRS})

PKG_CHECK_MODULES(libgnomecanvasmm REQUIRED libgnomecanvasmm-2.6)
include_directories(${libgnomecanvasmm_INCLUDE_DIRS})
link_directories(${libgnomecanvasmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkglextmm REQUIRED gtkglextmm-1.2)
include_directories(${gtkglextmm_INCLUDE_DIRS})
link_directories(${gtkglextmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gsl REQUIRED gsl)
include_directories(${gsl_INCLUDE_DIRS})
link_directories(${gsl_LIBRARIES_DIRS})

find_package(OpenCV REQUIRED)

add_executable (introrob  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(introrob 

    ${gtkmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${libglademm_LIBRARIES}
	${libgnomecanvasmm_LIBRARIES}
    ${OpenCV_LIBS}
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${INTERFACES_CPP_DIR}/jderobot/libjderobotice.so
    ${INTERFACES_CPP_DIR}/jderobot/libjderobotutil.so
    ${INTERFACES_CPP_DIR}/jderobot/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libprogeo.so
    ${INTERFACES_CPP_DIR}/jderobot/libpioneer.so
)
