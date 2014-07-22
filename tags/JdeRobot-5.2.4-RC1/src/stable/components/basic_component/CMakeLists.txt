
SET( SOURCE_FILES  control/control.cpp control/threadcontrol.cpp gui/gui.cpp gui/threadgui.cpp basic_component.cpp shared.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
)

add_executable (basic_component  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(basic_component 
    ${CMAKE_THREAD_LIBS_INIT}
    JderobotInterfaces
    jderobotutil
    colorspacesmm
    ${libglademm_LIBRARIES}
    ${OpenCV_LIBRARIES}       
    ${OpenCVGUI_LIBRARIES} 
	${ZeroCIce_LIBRARIES}    
	${gtkmm_LIBRARIES}  
	${gtkmm3_LIBRARIES}  	
	${gthread_LIBRARIES}
)

