FIND_PATH( kobuki_INCLUDE_DIR NAMES kobuki_driver/kobuki.hpp  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/include)

MESSAGE(" kobuki_LIBRARIES ${kobuki_LIBRARIES}")
#MESSAGE(" kobuki_INCLUDE_DIR ${kobuki_INCLUDE_DIR}")

IF (kobuki_INCLUDE_DIR)

	MESSAGE("kobuki was found in: ${kobuki_INCLUDE_DIR}")
    FIND_LIBRARY( kobuki_LIBRARY1 NAMES kobuki PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY2 NAMES kobuki_dock_drive PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    
    FIND_LIBRARY( kobuki_LIBRARY3 NAMES ecl_devices PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY4 NAMES ecl_errors PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY5 NAMES ecl_exceptions PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY6 NAMES ecl_formatters PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY7 NAMES ecl_geometry PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    
    FIND_LIBRARY( kobuki_LIBRARY8 NAMES ecl_io PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY9 NAMES ecl_ipc PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY10 NAMES ecl_mobile_robot PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY11 NAMES ecl_statistics PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY12 NAMES ecl_streams PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY13 NAMES ecl_threads PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY14 NAMES ecl_time PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    
    FIND_LIBRARY( kobuki_LIBRARY15 NAMES ecl_time_lite PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
    FIND_LIBRARY( kobuki_LIBRARY16 NAMES ecl_type_traits PATHS ENV C++LIB ENV PATH PATH_SUFFIXES kobuki/lib)
  #MESSAGE("PATH_SUFFIXES: ${PATH_SUFFIXES}")
    
    SET (kobuki_LIBRARIES ${kobuki_LIBRARY1} ${kobuki_LIBRARY2} ${kobuki_LIBRARY3} ${kobuki_LIBRARY4} ${kobuki_LIBRARY5} ${kobuki_LIBRARY6}
                          ${kobuki_LIBRARY7} ${kobuki_LIBRARY8} ${kobuki_LIBRARY9} ${kobuki_LIBRARY10} ${kobuki_LIBRARY11} ${kobuki_LIBRARY12}
                          ${kobuki_LIBRARY13} ${kobuki_LIBRARY14} ${kobuki_LIBRARY15} ${kobuki_LIBRARY16})
    
    IF( OpenCV_LIBRARIES )
		MESSAGE("kobuki LIBRARY FOUND IN ${kobuki_LIBRARIES}")
		SET(kobuki_COMPILE TRUE)
		SET(DEPS "${DEPS} kobuki,")
    else()
		MESSAGE("kobuki LIBRARY NOT FOUND IN ${kobuki_LIBRARIES}")
		SET(kobuki_COMPILE FALSE)
    ENDIF(  )

else() 
	MESSAGE("*** kobuki not found")
	SET(kobuki_COMPILE FALSE)
ENDIF()
