set(SLICE2CPPE /opt/IceE-1.3.0/bin/slice2cppe) 

set(JDEROBOT_ICEFILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/slice)
set(JDEROBOT_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/cpp/jderobot)

set (JDEROBOT_FILES
    camera
    common
    containers
    datetime
    exceptions
    image
	pose3dmotors
)

foreach (file ${JDEROBOT_FILES})
	set (jderobot_file jderobot/${file}.ice)
	set(args --output-dir=${JDEROBOT_OUTPUT_DIR} -I. -I${JDEROBOT_ICEFILES_DIR} ${JDEROBOT_ICEFILES_DIR}/${jderobot_file})
	execute_process(COMMAND ${SLICE2CPPE} ${args})	
endforeach()
