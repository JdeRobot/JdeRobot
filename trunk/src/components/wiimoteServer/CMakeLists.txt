IF (with_cwiid)


SET( SOURCE_FILES wiimoteServer.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas -lGL -lGLU -lglut  -lIce -lIceUtil" ) # Opciones para el compilador

add_executable (wiimoteServer ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(wiimoteServer
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${cwiid_LIBRARIES}
)

ENDIF(with_cwiid)