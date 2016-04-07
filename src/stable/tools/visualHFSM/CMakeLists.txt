IF (goocanvasmm_INCLUDE_DIRS)

SET( SOURCE_FILES_VISUALHFSM
	iceinterface.cpp
	point.cpp
	node.cpp
	transition.cpp
	guinode.cpp
	subautomata.cpp
	guitransition.cpp
	guisubautomata.cpp
	generate.cpp
	popups/configfiledialog.cpp
	popups/editnodedialog.cpp
	popups/edittransitiondialog.cpp
        popups/edittransitioncodedialog.cpp
	popups/funvardialog.cpp
	popups/librariesdialog.cpp
	popups/loadfiledialog.cpp
	popups/renamedialog.cpp
	popups/renametransitiondialog.cpp
	popups/savefiledialog.cpp
	popups/timerdialog.cpp
	savefile.cpp
	xmlparser.cpp
	visualhfsm.cpp
	main.cpp
)

include_directories (
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm3_INCLUDE_DIRS}
    ${goocanvasmm_INCLUDE_DIRS}
    ${gtk3_INCLUDE_DIRS}
    ${libxmlpp_INCLUDE_DIRS}
)

add_executable (visualHFSM ${SOURCE_FILES_VISUALHFSM})

TARGET_LINK_LIBRARIES ( visualHFSM 
    ${gtkmm3_LIBRARIES}
    ${goocanvasmm_LIBRARIES}
    ${gtk3_LIBRARIES}
    ${libxmlpp_LIBRARIES}
)


add_custom_command(TARGET visualHFSM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/gui $<TARGET_FILE_DIR:visualHFSM>/gui)

add_custom_command(TARGET visualHFSM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/getinterfaces.sh $<TARGET_FILE_DIR:visualHFSM>)
add_custom_command(TARGET visualHFSM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/allinterfaces.txt $<TARGET_FILE_DIR:visualHFSM>)

ENDIF()
