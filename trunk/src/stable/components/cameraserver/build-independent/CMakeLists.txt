cmake_minimum_required(VERSION 2.8)

SET( SOURCE_FILES ../cameraserver.cpp)
SET( INTERFACES_CPP_DIR /usr/local/lib)

include_directories(
	/usr/local/include/jderobot
	/usr/local/include/gearbox
)
SET( CMAKE_CXX_FLAGS "-lIce -lIceUtil" ) # Opciones para el compilador


add_executable (cameraserver ${SOURCE_FILES})

find_package(OpenCV REQUIRED)

TARGET_LINK_LIBRARIES(cameraserver 
	${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${INTERFACES_CPP_DIR}/jderobot/libjderobotice.so
    ${INTERFACES_CPP_DIR}/jderobot/libjderobotutil.so
	${INTERFACES_CPP_DIR}/jderobot/libcolorspacesmm.so
	${OpenCV_LIBS}
	/usr/local/lib/gearbox/libGbxIceUtilAcfr.so

)
	

