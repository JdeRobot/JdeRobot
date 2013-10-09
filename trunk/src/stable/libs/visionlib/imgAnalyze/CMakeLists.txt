SET( SOURCE_FILES geometry.cpp image.cpp linesDetection.cpp)
include_directories( ${LIBS_DIR}/)
add_library (imgAnalyze SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(imgAnalyze progeo cvfast)



