IF( with_player)

      SET( SOURCE_FILES playerserver.cpp)


      include_directories(
	  ${SLICE_DIR}
	  ${INTERFACES_CPP_DIR}
	  ${LIBS_DIR}
	  ${CMAKE_CURRENT_SOURCE_DIR}
	  /usr/local/include/player-2.1
      )

      set( CMAKE_CXX_FLAGS "-lplayerc")


      link_directories(/usr/local/lib)



      add_executable (playerserver ${SOURCE_FILES})


      TARGET_LINK_LIBRARIES(playerserver 
	  ${opencv_LIBRARIES}
	  ${gtkmm_LIBRARIES}
	  ${libglademm_LIBRARIES}
	  ${gthread_LIBRARIES}
	  ${libgnomecanvas_LIBRARIES}
	  ${libgnomecanvasmm_LIBRARIES}
	  ${gtkglextmm_LIBRARIES}
	  ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
	  ${LIBS_DIR}/jderobotice/libjderobotice.so
	  ${LIBS_DIR}/jderobotutil/libjderobotutil.so
          ${Gearbox_LIBRARIES}
          ${ZeroCIce_LIBRARIES}
 
	  
      )
ENDIF()
