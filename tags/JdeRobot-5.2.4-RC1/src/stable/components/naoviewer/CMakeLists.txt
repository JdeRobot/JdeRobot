IF (goocanvasmm_INCLUDE_DIRS)

SET( SOURCE_FILES_NAOOPERATOR sensors.cpp control.cpp naooperator.cpp displayer.cpp main.cpp )

SET( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador

include_directories (
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm3_INCLUDE_DIRS}
    ${gtk3_INCLUDE_DIRS}
    ${goocanvasmm_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

add_executable (naooperator ${SOURCE_FILES_NAOOPERATOR})

TARGET_LINK_LIBRARIES ( naooperator 
    JderobotInterfaces
    jderobotutil
    ${gtkmm3_LIBRARIES}
    ${gtk3_LIBRARIES}
    ${goocanvasmm_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    ${OpenCV_LIBRARIES}
    "dl"
    "rt"
    ${CMAKE_THREAD_LIBS_INIT}
)

ENDIF()
