CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
 
PROJECT(libgeometry)
 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} /usr/include/libxml2/)

set(SRC_FILES math/matriz3x3.cpp  math/matriz4x4.cpp  math/plano.cpp  math/utils.cpp  math/vector2d.cpp  math/vector3.cpp  math/vector2H.cpp math/vector3H.cpp math/Line2D.cpp math/Point2D.cpp math/Point3D.cpp collada/colladaparser.cpp collada/color.cpp collada/malla.cpp collada/material.cpp collada/submalla.cpp progeo/progeo2.cpp math/segmento.cpp math/recta.cpp) 
 
ADD_LIBRARY(geometry_shared SHARED ${SRC_FILES})

include(FindPkgConfig)
find_package(Boost COMPONENTS system filesystem REQUIRED)

target_link_libraries(geometry_shared tinyxml GLU opencv_core opencv_highgui ${Boost_LIBRARIES})

SET_PROPERTY(TARGET geometry_shared PROPERTY SOVERSION 1.0)
 
#ADD_LIBRARY(geometry_static STATIC ${SRC_FILES})

#target_link_libraries(geometry_shared tinyxml GLU ) 
#SET_TARGET_PROPERTIES(geometry_shared geometry_static PROPERTIES OUTPUT_NAME geometry)
