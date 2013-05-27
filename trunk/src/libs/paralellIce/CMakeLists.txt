include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
)

add_library (paralellIce STATIC cameraClient.cpp  cameraClient.h  pointcloudClient.cpp  pointcloudClient.h)
TARGET_LINK_LIBRARIES(paralellIce 
   	${OpenCV_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${ZeroCIce_LIBRARIES}
)

add_library (paralellIceshare SHARED cameraClient.cpp  cameraClient.h  pointcloudClient.cpp  pointcloudClient.h)
TARGET_LINK_LIBRARIES(paralellIceshare 
   	${OpenCV_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${ZeroCIce_LIBRARIES}
)


set_target_properties(paralellIceshare PROPERTIES OUTPUT_NAME paralellIce)

