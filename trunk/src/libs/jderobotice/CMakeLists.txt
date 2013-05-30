include_directories(${SLICE_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++
include_directories(${INTERFACES_CPP_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++
include_directories(${LIBS_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++

add_library (jderobotice STATIC component.cpp context.cpp application.cpp componentthread.cpp componentthread.h statusImpl.h tracerImpl.h catchutils.cpp subsystemthread.cpp interfaceconnect.cpp component.h context.h application.h exceptions.h catchutils.h subsystemthread.h interfaceconnect.h jderobotice.h)

TARGET_LINK_LIBRARIES(jderobotice ${Gearbox_LIBRARIES})
TARGET_LINK_LIBRARIES(jderobotice ${Gearbox_LIBRARIES})

add_library (jderoboticeshare SHARED component.cpp context.cpp application.cpp componentthread.cpp componentthread.h statusImpl.h tracerImpl.h catchutils.cpp subsystemthread.cpp interfaceconnect.cpp component.h context.h application.h exceptions.h catchutils.h subsystemthread.h interfaceconnect.h jderobotice.h)

TARGET_LINK_LIBRARIES(jderoboticeshare ${Gearbox_LIBRARIES})
TARGET_LINK_LIBRARIES(jderoboticeshare ${Gearbox_LIBRARIES})


set_target_properties(jderoboticeshare PROPERTIES OUTPUT_NAME jderobotice)

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
)

add_library (parallelIce STATIC cameraClient.cpp  cameraClient.h  pointcloudClient.cpp  pointcloudClient.h)
TARGET_LINK_LIBRARIES(parallelIce 
   	${OpenCV_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${ZeroCIce_LIBRARIES}
)

add_library (parallelIceshare SHARED cameraClient.cpp  cameraClient.h  pointcloudClient.cpp  pointcloudClient.h)
TARGET_LINK_LIBRARIES(parallelIceshare 
   	${OpenCV_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${ZeroCIce_LIBRARIES}
)


set_target_properties(parallelIceshare PROPERTIES OUTPUT_NAME parallelIce)

