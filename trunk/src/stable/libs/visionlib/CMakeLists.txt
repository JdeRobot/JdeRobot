list_subdirectories( LIST_LIBS ${CMAKE_CURRENT_SOURCE_DIR} 1)

FOREACH (libs ${LIST_LIBS})
        ADD_SUBDIRECTORY (${libs})
ENDFOREACH()

SET( SOURCE_FILES geometry.cpp image.cpp linesDetection.cpp)
include_directories( ${LIBS_DIR}/)
add_library (visionlib  SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(visionlib progeo cvfast)



