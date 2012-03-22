SET( SOURCE_FILES drawarea.cpp kinectViewer.cpp kinectViewergui.cpp myprogeo.cpp util3d.cpp)


include_directories(
    ${SLICE_DIR}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    /usr/local/include/player-2.1
)

set( CMAKE_CXX_FLAGS "-lIce -lIceUtil -lIceGrid -lIceBox -lIceStorm -lIceStormService -L/usr/lib/gearbox -lGbxUtilAcfr -lGbxIceUtilAcfr -pthread -lglademm-2.4 -lgtkmm-2.4 -lglade-2.0 -latkmm-1.6 -lgdkmm-2.4 -lgiomm-2.4 -lpangomm-1.4 -lglibmm-2.4 -lcairomm-1.0 -lsigc-2.0 -lgtk-x11-2.0 -lxml2 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0   -lcv -lhighgui -lcvaux -lml -lcxcore   -Wl,--export-dynamic -pthread -lgtkglextmm-x11-1.2 -lgdkglextmm-x11-1.2 -lgtkglext-x11-1.0 -lgtkmm-2.4 -lgdkglext-x11-1.0 -lGLU -lGL -lXmu -lXt -lSM -lICE -lpangox-1.0 -lX11 -latkmm-1.6 -lgdkmm-2.4 -lgiomm-2.4 -lpangomm-1.4 -lgtk-x11-2.0 -lglibmm-2.4 -lcairomm-1.0 -lsigc-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lm -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0   -lGLU -lGL   -lglut ")


#link_directories(/usr/local/lib)



add_executable (kinectViewer ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(kinectViewer 
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
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${LIBS_DIR}/progeo/libprogeo.so
   
)
