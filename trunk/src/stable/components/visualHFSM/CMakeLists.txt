cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

# SET( SOURCE_FILES_GENERATE reemplazar.cpp generate.cpp )
# SET( SOURCE_FILES_VISUALHFSM int2string.cpp recta.cpp nodo.cpp transicion.cpp ventanas.cpp xml.cpp interfaz.cpp )

SET( SOURCE_FILES_VISUALHFSM point.cpp node.cpp transition.cpp guinode.cpp subautomata.cpp guitransition.cpp guisubautomata.cpp generate.cpp popups/configfiledialog.cpp popups/editnodedialog.cpp popups/edittransitiondialog.cpp popups/funvardialog.cpp popups/importdialog.cpp popups/loadfiledialog.cpp popups/renamedialog.cpp popups/renametransitiondialog.cpp popups/savefiledialog.cpp popups/timerdialog.cpp savefile.cpp xmlparser.cpp visualhfsm.cpp main.cpp )

SET( INTERFACES_CPP_DIR /usr/local/lib )
SET( LIBS_DIR /usr/include )

SET( CMAKE_CXX_FLAGS "-g -Wall -fpermissive" ) # Opciones para el compilador

include_directories (
	/usr/include/gtksourceview-3.0
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
)

PKG_CHECK_MODULES(gtk30 REQUIRED gtk+-3.0)
include_directories(${gtk30_INCLUDE_DIRS})
link_directories(${gtk30_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkmm REQUIRED gtkmm-3.0)
include_directories(${gtkmm_INCLUDE_DIRS})
link_directories(${gtkmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(goocanvasmm REQUIRED goocanvasmm-2.0)
include_directories(${goocanvasmm_INCLUDE_DIRS})
link_directories(${goocanvasmm_LIBRARY_DIRS})

# PKG_CHECK_MODULES(libglade REQUIRED libglade-2.0)
# include_directories(${libglade_INCLUDE_DIRS})
# link_directories(${libglade_LIBRARY_DIRS})

# PKG_CHECK_MODULES(libgnomecanvas REQUIRED libgnomecanvas-2.0)
# include_directories(${libgnomecanvas_INCLUDE_DIRS})
# link_directories(${libgnomecanvas_LIBRARY_DIRS})

# PKG_CHECK_MODULES(libgnomecanvasmm REQUIRED libgnomecanvasmm-2.6)
# include_directories(${libgnomecanvasmm_INCLUDE_DIRS})
# link_directories(${libgnomecanvasmm_LIBRARY_DIRS})

# PKG_CHECK_MODULES(libxml REQUIRED libxml-2.0)
# include_directories(${libxml_INCLUDE_DIRS})
# link_directories(${libxml_LIBRARY_DIRS})

# find_package(LibXML++ REQUIRED)
# include_directories(${LibXML++_INCLUDE_DIRS})
# set(LIBS ${LIBS} ${LibXML++_LIBRARIES})

PKG_CHECK_MODULES(LibXML++ REQUIRED libxml++-2.6)
include_directories(${LibXML++_INCLUDE_DIRS})
link_directories(${LibXML++_LIBRARY_DIRS})

# PKG_CHECK_MODULES(gtk+-unix-print REQUIRED gtk+-unix-print-2.0)
# include_directories(${gtk+-unix-print_INCLUDE_DIRS})
# link_directories(${gtk+-unix-print_LIBRARY_DIRS})

# add_executable (generate ${SOURCE_FILES_GENERATE})

# TARGET_LINK_LIBRARIES ( generate
#    ${libxml_LIBRARIES}
# )

add_executable (../visualHFSM ${SOURCE_FILES_VISUALHFSM})

TARGET_LINK_LIBRARIES ( ../visualHFSM 
    ${gtkmm_LIBRARIES}
    ${goocanvasmm_LIBRARIES}
    ${gtk30_LIBRARIES}
    ${LibXML++_LIBRARIES}
    /usr/lib/libgtksourceview-3.0.so
)
