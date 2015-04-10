find_package(Doxygen)
# Show a message tip only first time
if(DOXYGEN_FOUND)
	message(STATUS "*** Found doxygen: use \"make doc\" to generate documentation")
	set(DOXYGEN_OUTPUT_BASE ${CMAKE_CURRENT_SOURCE_DIR}/doc)
	set(DOXYGEN_DEFAULT_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/Deps/doxygen/Doxyfile.in)
	message(STATUS "Documentation output directory: ${DOXYGEN_OUTPUT_BASE}")
	add_custom_target(doc
		COMMENT "Documentation generated"
	)
else(DOXYGEN_FOUND)
	message(STATUS "*** Doxygen NOT found: if you want to generate documentation you'll need to install Doxygen")
endif(DOXYGEN_FOUND)

