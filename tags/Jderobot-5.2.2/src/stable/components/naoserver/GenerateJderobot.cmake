##------------------------------------------------------------
## Author:		Gonzalo Abella (abellagonzalo@gmail.com)
## Update:		25/01/2012 by Gonzalo Abella
##------------------------------------------------------------

set(SLICE2CPPE /opt/IceE-1.3.0/bin/slice2cppe) 

### Bica for jderobot begin

set(JDEROBOT_ICEFILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/slice)
set(JDEROBOT_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/cpp/jderobot)

set (JDEROBOT_FILES
    camera
    common
    containers
    datetime
    encoders
    exceptions
    image
    motors
    naomotions
	pose3dmotors
	pose3dencoders
)

foreach (file ${JDEROBOT_FILES})
	set (jderobot_file jderobot/${file}.ice)
	set(args --output-dir=${JDEROBOT_OUTPUT_DIR} -I. -I${JDEROBOT_ICEFILES_DIR} ${JDEROBOT_ICEFILES_DIR}/${jderobot_file})
	execute_process(COMMAND ${SLICE2CPPE} ${args})	
endforeach()

#foreach (file ${JDEROBOT_FILES})
#	set (jderobot_file ${file})
#	get_filename_component (jderobot_file_name ${jderobot_file} NAME_WE)
	
#	set(args --output-dir=${JDEROBOT_OUTPUT_DIR} -I. -I${JDEROBOT_ICEFILES_DIR}/${jderobot_file})
	
#	message("args: " ${args})
	
#	ADD_CUSTOM_COMMAND (
# 		OUTPUT ${JDEROBOT_OUTPUT_DIR}/${jderobot_file_name}.cpp ${JDEROBOT_OUTPUT_DIR}/${jderobot_file_name}.h
#  		DEPENDS ${JDEROBOT_ICEFILES_DIR}/${jderobot_file}
#  		COMMAND ${SLICE2CPP} ${args}
#  	)
#endforeach()

### Bica for jderobot end

