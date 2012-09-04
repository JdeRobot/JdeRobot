SET( SOURCE_FILES wiimoteClient.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo -lIce -lIceUtil" ) # Opciones para el compilador

add_executable (wiimoteClient ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(wiimoteClient
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
)

