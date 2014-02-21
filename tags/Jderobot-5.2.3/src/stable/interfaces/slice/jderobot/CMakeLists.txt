#FIND_LIBRARY(ice_lib Ice)
#FIND_PATH(ice_path Ice)



#IF(NOT ice_path)
#  MESSAGE(FATAL_ERROR "** Librería de Ice no encontrada")
#ENDIF()

set( CMAKE_CXX_FLAGS "-lIce -lIceUtil -lIceGrid -lIceBox -lIceStorm -lIceStormService" ) # Opciones para el compilador

FILE(GLOB ICE_FILES "*.ice")

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../cpp/
  ${CMAKE_CURRENT_SOURCE_DIR}/../../cpp/jderobot
  ${ice_path_storm}
  )


FOREACH(currentSourceFile ${ICE_FILES})
	string(REGEX REPLACE ".*/(.*)" "\\1" new_source ${currentSourceFile})
	string(REGEX REPLACE ".*/(.*).ice" "\\1.cpp" new_source1 ${currentSourceFile})
	string(REGEX REPLACE ".*/(.*).ice" "\\1.h" new_source2 ${currentSourceFile})
	string(REGEX REPLACE ".*/(.*).ice" "\\1" new_source_name ${currentSourceFile})
	LIST(APPEND SOURCE_FILES ${new_source1})
	LIST(APPEND SOURCE_FILES ${new_source2})
	add_custom_command(OUTPUT ${new_source1} ${new_source2}
                   COMMAND slice2cpp -I${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/${new_source}
	)
	#add_library (${new_source_name} STATIC ${new_source1} ${new_soure_name2})
ENDFOREACH(currentSourceFile)


#MESSAGE("A:" ${SOURCE_FILES})

add_library (JderobotInterfaces SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(JderobotInterfaces ${ice2_LIBRARIES})
