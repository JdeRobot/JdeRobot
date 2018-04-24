include_directories(
	${GAZEBO_INCLUDE_DIRS}
        ${INTERFACES_CPP_DIR}
        ${LIBS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
	${easyiceconfig_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Headers trick for IDEs
file(GLOB_RECURSE roomba_headers "include/**")
add_library(roomba_headers ${roomba_headers})
set_target_properties(roomba_headers PROPERTIES LINKER_LANGUAGE CXX)

link_directories(
	${GAZEBO_LIBRARY_DIRS}
 	${easyiceconfig_LIBRARY_DIRS}
)

set(SOURCES src/roombaplugin.cc
        src/roombaice.cc

        src/roombacontrol.cc
        src/roombasensors.cc
        src/interfaces/motorsi.cpp
        src/interfaces/laseri.cpp
        src/interfaces/pose3di.cpp
        src/interfaces/bumperi.cpp
)

add_library(roombaplugin SHARED ${SOURCES})
# Gazebo
target_link_libraries(roombaplugin ${GAZEBO_libraries})
set_target_properties(roombaplugin PROPERTIES COMPILE_FLAGS "${GAZEBO_CXX_FLAGS}")

# Ice
target_link_libraries(roombaplugin
        Ice
        IceUtil
        JderobotInterfaces
        ${ZeroCIce_LIBRARIES}
        ${easyiceconfig_LIBRARIES}
)

# Application
target_link_libraries(roombaplugin
        colorspacesmm
)

set_target_properties(roombaplugin PROPERTIES COMPILE_FLAGS "-Wall -Wextra")
    
INSTALL (TARGETS roombaplugin DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/roomba/ COMPONENT gazebo)
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cfg/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT gazebo)

#MIO endif()
