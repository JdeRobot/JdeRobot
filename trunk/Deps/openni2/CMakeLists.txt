#manual openni libraries
SET ( OPENNI2_INC $ENV{OPENNI2_INCLUDE})
SET ( OPENNI2_LIB $ENV{OPENNI2_REDIST})

if (OPENNI2_INC AND OPENNI2_LIB)
	message(STATUS "OpenNI found at ${OPENNI2_LIB}")
	include_directories(${OPENNI2_INC})
	link_directories(${OPENNI2_LIB}/libOpenNI2.so)
	if (NOT IS_DIRECTORY ./OpenNI2)
		message ("--CMake will install local OpenNI2 lib and Driver")
		file (COPY ${OPENNI2_LIB}/libOpenNI2.so DESTINATION .)
		file (COPY ${OPENNI2_LIB}/OpenNI2 DESTINATION .)
	endif()
else()
	message("OpenNI2 library not found")
endif()

