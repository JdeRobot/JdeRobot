include_directories(
    ${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${easyiceconfig_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Headers trick for IDEs
file(GLOB_RECURSE formula1_headers "include/**")
add_library(formula1_headers ${formula1_headers})
set_target_properties(formula1_headers PROPERTIES LINKER_LANGUAGE CXX)

link_directories(
    ${GAZEBO_LIBRARY_DIRS}
    ${easyiceconfig_LIBRARY_DIRS}
)

#set( CMAKE_CXX_FLAGS "-Wall" )

set(SOURCES src/formula1plugin.cc
        src/formula1ice.cc

        src/formula1control.cc
        src/formula1sensors.cc

        src/cameraproxy.cc

        src/interfaces/motorsi.cpp
        src/interfaces/laseri.cpp
        src/interfaces/pose3di.cpp
        src/interfaces/camerai.cpp
        src/interfaces/pushcamerai.cpp
        src/interfaces/cameraibase.cpp
)

add_library(formula1plugin SHARED ${SOURCES})
# Gazebo
target_link_libraries(formula1plugin ${GAZEBO_libraries})
set_target_properties(formula1plugin PROPERTIES COMPILE_FLAGS "${GAZEBO_CXX_FLAGS}")

# Ice
target_link_libraries(formula1plugin
        Ice
        IceUtil
        JderobotInterfaces
        ${ZeroCIce_LIBRARIES}
        ${easyiceconfig_LIBRARIES}
)

# Application
target_link_libraries(formula1plugin
        ${OpenCV_LIBS}
        ${PCL_LIBRARIES}
        ${OpenCV_LIBRARIES}
        CameraPlugin
        colorspacesmm
)

INSTALL (TARGETS formula1plugin DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/formula1/ COMPONENT gazebo)
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cfg/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT gazebo)

