if (${QT_COMPILE} AND ${QWT_COMPILE})

	set(GUI_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/gui)

	MACRO (QT4_WRAP_UI_CUSTOM outfiles )

  		QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})

		FOREACH (it ${ui_files})
    		GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    		GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    		SET(outfile ${GUI_OUTPUT_PATH}/ui_${outfile}.h)
    		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      		COMMAND ${QT_UIC_EXECUTABLE}
      		ARGS ${ui_options} -o ${outfile} ${infile}
      		MAIN_DEPENDENCY ${infile})
    		SET(${outfiles} ${${outfiles}} ${outfile})
  		ENDFOREACH (it)

	ENDMACRO (QT4_WRAP_UI_CUSTOM)

	SET(qt_SOURCES ./sensors/threadsensors.cpp ./gui/threadgui.cpp ./gui/mainwindow.cpp 
		./gui/attitudeindicator.cpp ./gui/speedometer.cpp)

	SET(qt_HEADERS	./sensors/threadsensors.h ./gui/threadgui.h ./gui/mainwindow.h 
		./gui/attitudeindicator.h ./gui/speedometer.h)

	SET(qt_FORMS ./gui/mainwindow.ui)

	QT4_WRAP_CPP(qt_HEADERS_MOC ${qt_HEADERS})
	QT4_WRAP_UI_CUSTOM(qt_FORMS_HEADERS ${qt_FORMS})
	INCLUDE(${QT_USE_FILE})
	ADD_DEFINITIONS(${QT_DEFINITIONS})

	SET(SOURCE_FILES ./src/main.cpp ./sensors/sensors.cpp ./resources/images.cpp)

	include_directories(
		${OpenCV_INCLUDE_DIR}
        ${Ice_INCLUDE_DIR}
		${QWT_INCLUDE_DIR}
		${INTERFACES_CPP_DIR})

	link_directories(${JDE_LIBS})

	add_executable(uav_viewer 
		${SOURCE_FILES}
		${qt_SOURCES} 
		${qt_FORMS} 
        ${qt_HEADERS_MOC}
		${INTERFACES_CPP_DIR})

	target_link_libraries(uav_viewer
        ${CMAKE_THREAD_LIBS_INIT}
		${QT_LIBRARIES}
		${qwt_libraries}
		${OpenCV_LIBRARIES}
        ${OpenCVGUI_LIBRARIES}
        ${Ice_LIBRARIES}
		${ZeroCIce_LIBRARIES}
		JderobotInterfaces
		jderobotutil)

endif()
