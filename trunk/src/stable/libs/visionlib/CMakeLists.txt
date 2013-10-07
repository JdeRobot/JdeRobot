
add_subdirectory (colorspaces)

SET( SOURCE_FILES cvfast.cpp geometry.cpp image.cpp linesDetection.cpp)
include_directories( ${LIBS_DIR}/)
add_library (visionlib  SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(visionlib progeo)



