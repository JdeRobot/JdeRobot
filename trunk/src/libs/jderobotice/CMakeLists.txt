include_directories(${SLICE_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++
include_directories(${INTERFACES_CPP_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++
include_directories(${LIBS_DIR}) # Aquí se alojan las cabeceras de las interfaces ICE en C++

include_directories(/usr/include/gearbox)
link_directories(/usr/lib/gearbox)
#set( CMAKE_CXX_FLAGS "-lgsl -lgslcblas -lGL -lGLU -lglut -lgazebo -l/usr/lib/gearbox/libGbxUtilAcfr.so" ) # Opciones para el compilador
add_library (jderobotice STATIC component.cpp context.cpp application.cpp componentthread.cpp componentthread.h statusImpl.h tracerImpl.h catchutils.cpp subsystemthread.cpp interfaceconnect.cpp component.h context.h application.h exceptions.h catchutils.h subsystemthread.h interfaceconnect.h jderobotice.h)

#TARGET_LINK_LIBRARIES(jderobotice ${ice_LIBRARY_DIRS})
TARGET_LINK_LIBRARIES(jderobotice /usr/lib/gearbox/libGbxUtilAcfr.so)
TARGET_LINK_LIBRARIES(jderobotice /usr/lib/gearbox/libGbxIceUtilAcfr.so)

add_library (jderoboticeshare SHARED component.cpp context.cpp application.cpp componentthread.cpp componentthread.h statusImpl.h tracerImpl.h catchutils.cpp subsystemthread.cpp interfaceconnect.cpp component.h context.h application.h exceptions.h catchutils.h subsystemthread.h interfaceconnect.h jderobotice.h)

#TARGET_LINK_LIBRARIES(jderoboticeshare ${ice_LIBRARY_DIRS})
TARGET_LINK_LIBRARIES(jderoboticeshare /usr/lib/gearbox/libGbxUtilAcfr.so)
TARGET_LINK_LIBRARIES(jderoboticeshare /usr/lib/gearbox/libGbxIceUtilAcfr.so)


set_target_properties(jderoboticeshare PROPERTIES OUTPUT_NAME jderobotice)
