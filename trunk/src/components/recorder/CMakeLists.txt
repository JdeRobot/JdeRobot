SET( SOURCE_FILES  recorder.cpp)

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable (recorder ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(recorder 
    ${opencv_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
)
