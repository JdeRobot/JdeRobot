SET(cvFast_CVFAST cvfast.cpp)
add_library(cvfast SHARED ${cvFast_CVFAST})
TARGET_LINK_LIBRARIES(cvfast ${OPENGL_LIBRARIES})

