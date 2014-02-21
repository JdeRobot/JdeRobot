
add_library (colorspacesmm STATIC imagecv.cpp imagecv.h uncopyable.h)

TARGET_LINK_LIBRARIES(colorspacesmm ${OpenCV_LIBRARIES} ${OpenCVGUI_LIBRARIES})

add_library (colorspaces STATIC rgb2hsv.c rgb2yuv.c colorspaces.h colorspacesmm.h)

TARGET_LINK_LIBRARIES(colorspaces ${OpenCV_LIBRARIES})

add_library (colorspacesshare SHARED rgb2hsv.c rgb2yuv.c colorspaces.h colorspacesmm.h)

TARGET_LINK_LIBRARIES(colorspacesshare  ${OpenCV_LIBRARIES})

add_library (colorspacesmmshare SHARED imagecv.cpp imagecv.h uncopyable.h)

TARGET_LINK_LIBRARIES(colorspacesmmshare ${OpenCV_LIBRARIES} ${OpenCVGUI_LIBRARIES})

set_target_properties(colorspacesmmshare PROPERTIES OUTPUT_NAME colorspacesmm)
set_target_properties(colorspacesshare PROPERTIES OUTPUT_NAME colorspaces)
