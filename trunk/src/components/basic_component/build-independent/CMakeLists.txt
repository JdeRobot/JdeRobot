cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

SET( SOURCE_FILES ../control.cpp ../basic_component.cpp ../API.cpp ../gui.cpp)

SET(INTERFACES_CPP_DIR /usr/local/lib )
SET( CMAKE_CXX_FLAGS "-lIce -lIceUtil" ) # Opciones para el compilador

include_directories(
    ${INTERFACES_CPP_DIR}
    /usr/local/include/jderobot/
)


PKG_CHECK_MODULES(gtkmm REQUIRED gtkmm-2.4)
include_directories(${gtkmm_INCLUDE_DIRS})
link_directories(${gtkmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gthread REQUIRED gthread-2.0)
include_directories(${gthread_INCLUDE_DIRS})
link_directories(${gthread_LIBRARY_DIRS})

PKG_CHECK_MODULES(libglademm REQUIRED libglademm-2.4)
include_directories(${libglademm_INCLUDE_DIRS})
link_directories(${libglademm_LIBRARY_DIRS})

find_package(OpenCV REQUIRED)

add_executable (basic_component  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(basic_component 
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${INTERFACES_CPP_DIR}/jderobot/libjderobotice.so
    ${INTERFACES_CPP_DIR}/jderobot/libjderobotutil.so
    ${INTERFACES_CPP_DIR}/jderobot/libcolorspacesmm.so
    ${libglademm_LIBRARIES}
    ${OpenCV_LIBS}
#	${gtkmm_LIBRARIES}  
#	${gthread_LIBRARIES}
)
