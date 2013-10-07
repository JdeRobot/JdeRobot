
SET( SOURCE_FILES  control.cpp basic_component.cpp API.cpp gui.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable (basic_component  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(basic_component 
    ${CMAKE_THREAD_LIBS_INIT}
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${libglademm_LIBRARIES}
    ${OpenCV_LIBRARIES}       
    ${OpenCVGUI_LIBRARIES} 
	${ZeroCIce_LIBRARIES}    
	${gtkmm_LIBRARIES}  
	${gtkmm3_LIBRARIES}  	
	${gthread_LIBRARIES}
)

