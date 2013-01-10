cmake_minimum_required(VERSION 2.8)

  SET( SOURCE_FILES playerserver4.cpp)
	#SET( LIBS_DIR /usr/local/lib/jderobot/)

  include_directories(
    ${SLICE_DIR}
	${LIBS_DIR}/
	${INTERFACES_CPP_DIR}
  /usr/local/include/jderobot/
  /usr/local/include/player-3.0
  /usr/local/include/gearbox
  )


  set( CMAKE_CXX_FLAGS "-lplayerc -lplayerc++ -lplayercommon -lplayercore  -lIceUtil -lIce")

  link_directories(/usr/local/lib)

  add_executable (playerserver ${SOURCE_FILES})

  TARGET_LINK_LIBRARIES(playerserver 
		${Gearbox_LIBRARIES}
		${ZeroCIce_LIBRARIES}
		${Player_LIBRARIES}
	    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
	    ${LIBS_DIR}/jderobotice/libjderobotice.so
	    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
#		${LIBS_DIR}/libJderobotInterfaces.so
#		${LIBS_DIR}/libjderobotice.so
#		${LIBS_DIR}/libjderobotutil.so
  )

