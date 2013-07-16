


add_custom_command(OUTPUT myInterface.cpp myInterface.h
                   COMMAND slice2cpp myInterface.ice
	)

SET( SOURCE_FILES  basic_server.cpp myInterface.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable (basic_server  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(basic_server 
	${ZeroCIce_LIBRARIES}    	
)
