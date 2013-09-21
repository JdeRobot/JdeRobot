include_directories(${SLICE_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++
include_directories(${INTERFACES_CPP_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++
include_directories(${LIBS_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++


include_directories( ${LIBS_DIR}/)

add_library (parallelIce STATIC cameraClient.cpp  cameraClient.h  pointcloudClient.cpp  pointcloudClient.h)
TARGET_LINK_LIBRARIES(parallelIce colorspacesmm)

add_library (parallelIceshare SHARED cameraClient.cpp  cameraClient.h  pointcloudClient.cpp  pointcloudClient.h)
TARGET_LINK_LIBRARIES(parallelIceshare colorspacesmm)


set_target_properties(parallelIceshare PROPERTIES OUTPUT_NAME parallelIce)
