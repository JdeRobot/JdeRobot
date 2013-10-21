# CHECK NITE
SET ( NITE2_INC $ENV{NITE2_INCLUDE})
SET ( NITE2_LIB $ENV{NITE2_REDIST})

if (NITE2_INC AND NITE2_LIB)
	message(STATUS "NITE found at ${NITE2_LIB}")
	set_property(
  		SOURCE openniServer.cpp
   	PROPERTY COMPILE_DEFINITIONS WITH_NITE2=1
	include_directories(${NITE2_INC})
	link_directories(${NITE2_LIB}/libNiTE2.so)
	if (NOT IS_DIRECTORY ./NiTE2)
		message ("--CMake will install local NiTE2 lib and Driver")
		file (COPY ${NITE2_LIB}/libNiTE2.so DESTINATION .)
		file (COPY ${NITE2_LIB}/NiTE2 DESTINATION .)
	endif()

   	)
else()
	message("NITE2 library not found")
endif()



