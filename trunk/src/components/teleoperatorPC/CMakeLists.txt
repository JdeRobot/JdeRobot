
SET( SOURCE_FILES teleoperatorPC.cpp SharedMemory.cpp Gui.cpp controlICE.cpp)

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set( CMAKE_CXX_FLAGS "-lglut -lgsl -lgslcblas -g" ) # Opciones para el compilador

add_executable (teleoperatorPC  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(teleoperatorPC 
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${OpenCV_LIBRARIES}        
	${ZeroCIce_LIBRARIES}    
	${gtkmm_LIBRARIES}  
	${gtkmm3_LIBRARIES}  	
	${gthread_LIBRARIES}
)

