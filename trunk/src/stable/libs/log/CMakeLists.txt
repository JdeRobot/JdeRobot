

set(SRC_FILES Logger.h Logger.cpp) 
 
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Si el compilador es superior a 4.6 debemos incluir una opción adicional
  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.6)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
endif()

ADD_LIBRARY(logger SHARED ${SRC_FILES})

target_link_libraries(logger ${Boost_LIBRARIES})

SET_PROPERTY(TARGET logger PROPERTY SOVERSION 0.1.0)
 
