SET (COMPILEFORNAO FALSE)

if (COMPILEFORNAO)
    cmake_minimum_required(VERSION 2.8)
    
    add_definitions( -DCOMPILEFORNAO )
    include(qibuild.cmake)

    # Generar los ficheros de JdeRobot. 
    #Los ficheros se crean los respectivos directorios
    # en src/interfaces/cpp/ .
    include(GenerateJderobot.cmake)

    # Todos los directorios con headers
    include_directories(
	    interfaces/cpp/jderobot 
	    interfaces/slice/jderobot 
	    interfaces/cpp 
	    interfaces/slice
	    include
    )

    # Todas las fuentes del proyecto
    set(_srcs
    interfaces/cpp/jderobot/camera.cpp interfaces/cpp/jderobot/camera.h
    interfaces/cpp/jderobot/common.cpp interfaces/cpp/jderobot/common.h
    interfaces/cpp/jderobot/containers.cpp interfaces/cpp/jderobot/containers.h
    interfaces/cpp/jderobot/datetime.cpp interfaces/cpp/jderobot/datetime.h
    interfaces/cpp/jderobot/exceptions.cpp interfaces/cpp/jderobot/exceptions.h
    interfaces/cpp/jderobot/image.cpp interfaces/cpp/jderobot/image.h
    interfaces/cpp/jderobot/pose3dmotors.cpp interfaces/cpp/jderobot/pose3dmotors.h

    sensors.cpp sensors.h
    control.cpp control.h
    handler.cpp handler.h
    main.cpp
    )

    qi_create_bin(naofollowball ${_srcs})

    qi_use_lib(naofollowball LIBICEE OPENCV)
else()
    SET( SOURCE_FILES_FOLLOWBALL viewer.cpp sensors.cpp control.cpp handler.cpp main.cpp )

    SET( CMAKE_CXX_FLAGS "-lpthread -lIce" ) # Opciones para el compilador

    include_directories (
        ${INTERFACES_CPP_DIR}
        ${LIBS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${gtk20_INCLUDE_DIRS}
        ${gtkmm_INCLUDE_DIRS}
        ${libglademm_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
    )

    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

    add_executable (followball ${SOURCE_FILES_FOLLOWBALL})

    TARGET_LINK_LIBRARIES ( followball
        JderobotInterfaces
        jderobotutil
        ${gtk20_LIBRARIES}
        ${gtkmm_LIBRARIES}
        ${libglademm_LIBRARIES}
        ${ZeroCIce_LIBRARIES}
        ${OpenCV_LIBRARIES}
    )
endif()
