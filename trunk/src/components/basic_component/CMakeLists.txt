
SET( SOURCE_FILES  control.cpp basic_component.cpp API.cpp gui.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set( CMAKE_CXX_FLAGS "-lglut -lgsl -lgslcblas -g" ) # Opciones para el compilador

add_executable (basic_component  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(basic_component 
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${libglademm_LIBRARIES}
    ${OpenCV_LIBRARIES}        
	${ZeroCIce_LIBRARIES}    
	${gtkmm_LIBRARIES}  
	${gtkmm3_LIBRARIES}  	
	${gthread_LIBRARIES}
)

