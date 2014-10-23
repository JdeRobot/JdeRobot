

set(SRC_FILES Logger.h Logger.cpp) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
 
ADD_LIBRARY(logger SHARED ${SRC_FILES})

target_link_libraries(logger ${Boost_LIBRARIES})

SET_PROPERTY(TARGET logger PROPERTY SOVERSION 0.1.0)
 
