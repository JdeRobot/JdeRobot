if (${QT_COMPILE})

    #Generating Qt shared library
    SET(qt_SOURCES gui/gui_qt/gui.cc)
    SET(qt_HEADERS gui/gui_qt/gui.h)

    QT4_WRAP_CPP(qt_HEADERS_MOC ${qt_HEADERS})

    add_library(guiqt SHARED gui/gui_qt/gui.cc ${qt_HEADERS_MOC})
    target_link_libraries (guiqt ${QT_LIBRARIES_JDE})
    
endif()		
	       
#Generating gtk shared library
add_library(guigtk SHARED gui/gui_gtk/gui.cc)
target_link_libraries (guigtk
    ${libglademm_LIBRARIES}
    ${gtkmm_LIBRARIES}  
    ${gtkmm3_LIBRARIES}  	
    ${gthread_LIBRARIES}
)

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
)

#GENERAL COMPILATION (NO DEPENDENCIES)
add_executable( basic_component
	basic_component.cpp 
        control/threadcontrol.cpp 
        control/control.cpp
        gui/threadgui.cpp
        shared.cpp		    
)


target_link_libraries(basic_component 
        ${CMAKE_THREAD_LIBS_INIT}
	JderobotInterfaces
   	jderobotutil
    	colorspacesmm
    	${OpenCV_LIBRARIES}       
    	${OpenCVGUI_LIBRARIES} 
    	${ZeroCIce_LIBRARIES}   
    	guigtk dl
    )

if (${QT_COMPILE})
    target_link_libraries(basic_component guiqt dl)
endif()
