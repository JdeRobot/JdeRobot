if (${QT_COMPILE})


    SET(qt_SOURCES main.cpp 
			       sensors/threadsensors.cpp sensors/sensors.cpp
			       gui/threadgui.cpp gui/gui.cpp
    )

    SET(qt_HEADERS
			       sensors/threadsensors.h sensors/sensors.h
			       gui/threadgui.h gui/gui.h

    )

    include_directories(
        ${INTERFACES_CPP_DIR}
        ${LIBS_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    QT4_WRAP_CPP(qt_HEADERS_MOC ${qt_HEADERS})


    add_executable( basic_component_qt
                    main.cpp 
			        sensors/threadsensors.cpp sensors/sensors.cpp
			        gui/threadgui.cpp gui/gui.cpp
			        ${qt_HEADERS_MOC}
    )
			       
    target_link_libraries(basic_component_qt 
        ${CMAKE_THREAD_LIBS_INIT}
        ${OpenCV_LIBRARIES}
	    ${QT_LIBRARIES_JDE}
        JderobotInterfaces
        jderobotutil
        ${ZeroCIce_LIBRARIES}
    )
ENDIF()

