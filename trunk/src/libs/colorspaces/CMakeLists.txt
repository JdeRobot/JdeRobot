
add_library (colorspacesmm STATIC imagecv.cpp imagecv.h uncopyable.h)

add_library (colorspaces STATIC rgb2hsv.c rgb2yuv.c colorspaces.h colorspacesmm.h)

add_library (colorspacesshare SHARED rgb2hsv.c rgb2yuv.c colorspaces.h colorspacesmm.h)

add_library (colorspacesmmshare SHARED imagecv.cpp imagecv.h uncopyable.h)

set_target_properties(colorspacesmmshare PROPERTIES OUTPUT_NAME colorspacesmm)
set_target_properties(colorspacesshare PROPERTIES OUTPUT_NAME colorspaces)
