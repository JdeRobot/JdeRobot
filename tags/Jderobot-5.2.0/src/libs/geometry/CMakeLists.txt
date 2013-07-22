CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
 
PROJECT(libgeometry)
 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
 
ADD_LIBRARY(geometry_shared SHARED math/matriz3x3.cpp math/matriz4x4.cpp math/utils.cpp math/vector2d.cpp math/vector3.cpp
                                math/segmento.cpp math/recta.cpp math/plano.cpp
                                collada/colladaparser.cpp collada/color.cpp collada/malla.cpp collada/material.cpp collada/submalla.cpp)

SET_PROPERTY(TARGET geometry_shared PROPERTY SOVERSION 1.0)
 
ADD_LIBRARY(geometry_static STATIC math/matriz3x3.cpp math/matriz4x4.cpp math/utils.cpp math/vector2d.cpp math/vector3.cpp
                                math/segmento.cpp math/recta.cpp math/plano.cpp
                                collada/colladaparser.cpp collada/color.cpp collada/malla.cpp collada/material.cpp collada/submalla.cpp)
 
SET_TARGET_PROPERTIES(geometry_shared geometry_static PROPERTIES OUTPUT_NAME geometry)
