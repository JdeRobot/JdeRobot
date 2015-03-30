#to fetch installed version of opencv 3 packages
find_package( OpenCV REQUIRED )
add_library (colorspacesmm STATIC imagecv.cpp imagecv.h uncopyable.h)

TARGET_LINK_LIBRARIES(${OpenCV_CFLAGS} colorspacesmm  ${OpenCV_LIBS}	 ${OpenCV_LIBRARIES} ${OpenCVGUI_LIBRARIES})

add_library (colorspaces STATIC rgb2hsv.c rgb2yuv.c colorspaces.h colorspacesmm.h)

TARGET_LINK_LIBRARIES(${OpenCV_CFLAGS} colorspaces  ${OpenCV_LIBS} ${OpenCV_LIBRARIES})

add_library (colorspacesshare SHARED rgb2hsv.c rgb2yuv.c colorspaces.h colorspacesmm.h)

TARGET_LINK_LIBRARIES(${OpenCV_CFLAGS} colorspacesshare  ${OpenCV_LIBS}  ${OpenCV_LIBRARIES})

add_library (colorspacesmmshare SHARED imagecv.cpp imagecv.h uncopyable.h)

TARGET_LINK_LIBRARIES(${OpenCV_CFLAGS} colorspacesmmshare  ${OpenCV_LIBS} ${OpenCV_LIBRARIES} ${OpenCVGUI_LIBRARIES})

set_target_properties(colorspacesmmshare PROPERTIES OUTPUT_NAME colorspacesmm)
set_target_properties(colorspacesshare PROPERTIES OUTPUT_NAME colorspaces)
