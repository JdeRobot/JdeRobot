SET( SOURCE_FILES jointmotorComp.cpp jointmotorI.cpp worker.cpp dynamixel.cpp monitor.cpp servo.cpp q4serialport/q4serialport.cpp moc_monitor.cpp  moc_q4serialport.cpp  moc_worker.cpp)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
)

add_executable (giraffeServer ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(giraffeServer
    ${CMAKE_THREAD_LIBS_INIT}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${ZeroCIce_LIBRARIES}   
    ${QT_LIBRARIES_JDE}
    JderobotInterfaces
    QtCore
    QtGui
)
