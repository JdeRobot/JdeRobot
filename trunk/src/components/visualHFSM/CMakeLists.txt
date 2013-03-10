cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

SET( SOURCE_FILES_GENERATE reemplazar.cpp generate.cpp )
SET( SOURCE_FILES_VISUALHFSM int2string.cpp recta.cpp nodo.cpp transicion.cpp ventanas.cpp xml.cpp interfaz.cpp )

SET( INTERFACES_CPP_DIR /usr/local/lib )
SET( LIBS_DIR /usr/include )

SET( CMAKE_CXX_FLAGS "-g -Wall -fpermissive" ) # Opciones para el compilador

include_directories (
	/usr/include/gtksourceview-2.0
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
)

PKG_CHECK_MODULES(gtk20 REQUIRED gtk+-2.0)
include_directories(${gtk20_INCLUDE_DIRS})
link_directories(${gtk20_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkmm REQUIRED gtkmm-2.4)
include_directories(${gtkmm_INCLUDE_DIRS})
link_directories(${gtkmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(libglademm REQUIRED libglademm-2.4)
include_directories(${libglademm_INCLUDE_DIRS})
link_directories(${libglademm_LIBRARY_DIRS})

PKG_CHECK_MODULES(libglade REQUIRED libglade-2.0)
include_directories(${libglade_INCLUDE_DIRS})
link_directories(${libglade_LIBRARY_DIRS})

PKG_CHECK_MODULES(libgnomecanvas REQUIRED libgnomecanvas-2.0)
include_directories(${libgnomecanvas_INCLUDE_DIRS})
link_directories(${libgnomecanvas_LIBRARY_DIRS})

PKG_CHECK_MODULES(libgnomecanvasmm REQUIRED libgnomecanvasmm-2.6)
include_directories(${libgnomecanvasmm_INCLUDE_DIRS})
link_directories(${libgnomecanvasmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(libxml REQUIRED libxml-2.0)
include_directories(${libxml_INCLUDE_DIRS})
link_directories(${libxml_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtk+-unix-print REQUIRED gtk+-unix-print-2.0)
include_directories(${gtk+-unix-print_INCLUDE_DIRS})
link_directories(${gtk+-unix-print_LIBRARY_DIRS})

message(${gtk20_LIBRARIES})

add_executable (generate  ${SOURCE_FILES_GENERATE})

TARGET_LINK_LIBRARIES ( generate
    ${libxml_LIBRARIES}
)

add_executable (visualHFSM ${SOURCE_FILES_VISUALHFSM})

TARGET_LINK_LIBRARIES ( visualHFSM 
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${libglade_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${gtk+-unix-print_LIBRARIES}
    ${gtk20_LIBRARIES}
    ${libxml_LIBRARIES}
    /usr/lib/libgtksourceview-2.0.so
)
