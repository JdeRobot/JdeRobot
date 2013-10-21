##------------------------------------------------------------
## Author:		Gonzalo Abella (abellagonzalo@gmail.com)
## Update:		25/01/2012 by Gonzalo Abella
##------------------------------------------------------------

set(SLICE2CPPE ${PROJECT_SOURCE_DIR}/bin/slice2cppe) 

### Bica for jderobot begin

set(JDEROBOT_ICEFILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/slice)
set(JDEROBOT_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/cpp/jderobot)

set (JDEROBOT_FILES
	pose3dmotors
	pose3dencoders
)

foreach (file ${JDEROBOT_FILES})
	set (jderobot_file jderobot/${file}.ice)
	set(args --output-dir=${JDEROBOT_OUTPUT_DIR} -I. -I${JDEROBOT_ICEFILES_DIR} ${JDEROBOT_ICEFILES_DIR}/${jderobot_file})
	execute_process(COMMAND ${SLICE2CPPE} ${args})	
endforeach()

### Bica for jderobot end

