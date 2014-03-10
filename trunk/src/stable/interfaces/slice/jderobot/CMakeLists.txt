
# Check the ice binaries

set( SLICE2PY_COMMAND /usr/bin/slice2py)

set( CMAKE_CXX_FLAGS "-lIce -lIceUtil -lIceGrid -lIceBox -lIceStorm -lIceStormService" ) # Opciones para el compilador

FILE(GLOB ICE_FILES "*.ice")

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../cpp/
  ${CMAKE_CURRENT_SOURCE_DIR}/../../cpp/jderobot/
  ${ice_path_storm}
  )



FOREACH(currentSourceFile ${ICE_FILES})
	string(REGEX REPLACE ".*/(.*)" "\\1" new_source ${currentSourceFile})
	string(REGEX REPLACE ".*/(.*).ice" "\\1.cpp" new_source1 ${currentSourceFile})
	string(REGEX REPLACE ".*/(.*).ice" "\\1.h" new_source2 ${currentSourceFile})
	string(REGEX REPLACE ".*/(.*).ice" "\\1.py" SOURCE_PY ${currentSourceFile})
	string(REGEX REPLACE ".*/(.*).ice" "\\1" new_source_name ${currentSourceFile})
	LIST(APPEND SOURCE_FILES ${new_source1})
	LIST(APPEND SOURCE_FILES ${new_source2})

	#MESSAGE(STATUS, "source1= ${new_source1}  source2= ${new_source2}  source=${new_source}")

	# C++
	add_custom_command(OUTPUT ${new_source1} ${new_source2}
                   COMMAND slice2cpp -I${CMAKE_CURRENT_SOURCE_DIR}/.. -I${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${new_source}
	)

        # Python
	execute_process(COMMAND ${SLICE2PY_COMMAND} -I${CMAKE_CURRENT_SOURCE_DIR}/.. -I${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${new_source} --output-dir ${CMAKE_CURRENT_SOURCE_DIR}/../../python/jderobot)

ENDFOREACH(currentSourceFile)


#MESSAGE("A:" ${SOURCE_FILES})

add_library (JderobotInterfaces SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(JderobotInterfaces ${ice2_LIBRARIES})
