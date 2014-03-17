
SET( SOURCE_FILES main.cpp NamingServiceJdeRobot.h NamingServiceJdeRobot.cpp)

add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}    
)

add_executable (namingService ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(namingService
    ${CMAKE_THREAD_LIBS_INIT}
    ${gsl_LIBRARIES}    
    ${ZeroCIce_LIBRARIES}
    ${Boost_LIBRARIES}    
    JderobotInterfaces
    logger
)
