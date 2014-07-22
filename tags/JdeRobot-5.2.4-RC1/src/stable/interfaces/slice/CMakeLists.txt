find_file(interfaces_exists NAMES libJderobotInterfaces.so PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../cpp/jderobot)
string(COMPARE EQUAL ${interfaces_exists} "interfaces_exists-NOTFOUND" result)
IF(result)
    add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/jderobot ${CMAKE_CURRENT_SOURCE_DIR}/../cpp/jderobot)
    MESSAGE (STATUS "Creating CPP interfaces") 
ENDIF()

