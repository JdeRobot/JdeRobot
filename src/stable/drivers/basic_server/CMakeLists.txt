


execute_process(COMMAND slice2cpp ${CMAKE_CURRENT_LIST_DIR}/myInterface.ice WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

SET( SOURCE_FILES  basic_server.cpp myInterface.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable (basic_server  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(basic_server
        ${CMAKE_THREAD_LIBS_INIT}
	${ZeroCIce_LIBRARIES}    	
)
