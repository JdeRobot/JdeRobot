SET( SOURCE_FILES  opencvdemo.cpp viewer.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
)


add_executable (opencvdemo ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(opencvdemo
    ${CMAKE_THREAD_LIBS_INIT} 
    ${OpenCV_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
    ${ZeroCIce_LIBRARIES}
)

# Generate documentation if doxygen was found
if(DOXYGEN_FOUND)
    get_filename_component(DOC_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    set(DOXYGEN_INPUT ${DOXYGEN_DEFAULT_INPUT})
    set(DOXYGEN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT_BASE}/components/${DOC_NAME}/)
    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT})
    configure_file(${DOXYGEN_INPUT} ${DOXYGEN_FILE} @ONLY)
    add_custom_target(doc_${DOC_NAME} ${DOXYGEN_EXECUTABLE} ${DOXYGEN_FILE} | grep "Parsing file"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating: component ${DOC_NAME}" VERBATIM
        DEPENDS ${DOXYGEN_INPUT}
    )
    add_dependencies(doc doc_${DOC_NAME})
endif(DOXYGEN_FOUND)
