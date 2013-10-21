#SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../Deps) # Directorio donde se encuentran las dependencias
#include(${DEPS_DIR}/openni2/CMakeLists.txt)
#include(${DEPS_DIR}/nite2/CMakeLists.txt)

IF(OPENNI2_LIB)
	#check for the path openni libs
	


	SET( SOURCE_FILES openniServer.cpp myprogeo.cpp )
    add_definitions(-DGLADE_DIR="${gladedir}")

	set( CMAKE_CXX_FLAGS "-Wno-deprecated -Wl,-rpath ./" ) # Opciones para el compilador

    include_directories(
        ${INTERFACES_CPP_DIR}
        ${LIBS_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/cameras
    )

    add_executable (openniServer ${SOURCE_FILES})
	#copy openni libs to local path
	if (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/OpenNI2)
		message ("--CMake will install local OpenNI2 lib and Driver")
		file (COPY ${OPENNI2_LIB}/libOpenNI2.so DESTINATION .)
		set (concat "/")
		file (COPY ${OPENNI2_LIB}/OpenNI2 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
	endif()
	message ("-----------------------------------" ${LIBS_DIR})


	if  ( NITE2_LIB )
		#copy nite libs to local path
		if (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/NiTE2)
			message ("--CMake will install local NiTE2 lib and Driver")
			file (COPY ${NITE2_LIB}/libNiTE2.so DESTINATION .)
			file (COPY ${NITE2_LIB}/NiTE2 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
		endif()
	TARGET_LINK_LIBRARIES(openniServer
        ${CMAKE_THREAD_LIBS_INIT}
	${CMAKE_THREAD_LIBS_INIT}
        ${ZeroCIce_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${OpenCVGUI_LIBRARIES} 
        ${gsl_LIBRARIES}
        jderobotutil
        progeo
        colorspacesmm
        JderobotInterfaces
	${OPENNI2_LIB}/libOpenNI2.so
	${NITE2_LIB}/libNiTE2.so
	${Boost_LIBRARIES}
        ${LIBXML2_LIBRARIES}
	)
	else()
	TARGET_LINK_LIBRARIES(openniServer
        ${CMAKE_THREAD_LIBS_INIT}
	${ZeroCIce_LIBRARIES}
	${OpenCV_LIBRARIES}
        ${OpenCVGUI_LIBRARIES}
	${gsl_LIBRARIES}
        jderobotutil
        progeo
        colorspacesmm
        JderobotInterfaces
	${OPENNI2_LIB}/libOpenNI2.so
        ${Boost_LIBRARIES}
        ${LIBXML2_LIBRARIES}
	)
	endif()
ENDIF()
