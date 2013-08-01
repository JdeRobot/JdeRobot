CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
 
PROJECT(libgeometry)
 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} /usr/include/libxml2/)
 
ADD_LIBRARY(geometry_shared SHARED math/matriz3x3.cpp  math/matriz4x4.cpp  math/plano.cpp  math/recta.cpp  
				   math/segmento.cpp  math/utils.cpp  math/vector2d.cpp  math/vector2H.cpp  math/vector3.cpp  math/vector3H.cpp
				   math/Line2D.cpp 
                                   collada/colladaparser.cpp collada/color.cpp collada/malla.cpp collada/material.cpp collada/submalla.cpp progeo/progeo.cpp)

target_link_libraries(geometry_shared tinyxml GLU )

SET_PROPERTY(TARGET geometry_shared PROPERTY SOVERSION 1.0)
 
ADD_LIBRARY(geometry_static STATIC math/matriz3x3.cpp  math/matriz4x4.cpp  math/plano.cpp  math/recta.cpp  
                                   math/segmento.cpp  math/utils.cpp  math/vector2d.cpp  math/vector2H.cpp  math/vector3.cpp  math/vector3H.cpp 
                                   math/Line2D.cpp
                                   collada/colladaparser.cpp collada/color.cpp collada/malla.cpp collada/material.cpp collada/submalla.cpp progeo/progeo.cpp)

target_link_libraries(geometry_shared tinyxml GLU ) 
SET_TARGET_PROPERTIES(geometry_shared geometry_static PROPERTIES OUTPUT_NAME geometry)
