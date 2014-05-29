if (${QT_COMPILE})


    SET(qt_SOURCES basic_component_qt.cpp gui/gui.cpp)

    SET(qt_HEADERS gui/gui.h)

    include_directories(
        ${INTERFACES_CPP_DIR}
        ${LIBS_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    QT4_WRAP_CPP(qt_HEADERS_MOC ${qt_HEADERS})


    add_executable( basic_component_qt
                    basic_component_qt.cpp 
	            control/threadcontrol.cpp 
		    control/control.cpp
		    gui/threadgui.cpp 
		    gui/gui.cpp 
		    shared.cpp
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

