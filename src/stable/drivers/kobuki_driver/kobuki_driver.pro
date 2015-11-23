SOURCES += \
    main.cpp \
    kobukimanager.cpp \
    sensors/encoders.cpp \
    actuators/motors.cpp \
    thread_control.cpp
HEADERS += \
    kobukimanager.h \
    sensors/encoders.h \
    actuators/motors.h \
    thread_control.h

INCLUDEPATH+=/usr/local/kobuki/include/ \
    /usr/include/eigen3/

LIBS+=-L/usr/local/kobuki/install/lib \
    -lecl_devices     -lecl_ipc           -lecl_time_lite \
    -lecl_errors      -lecl_mobile_robot  -lecl_type_traits \
    -lecl_exceptions  -lecl_statistics    -lkobuki \
    -lecl_formatters  -lecl_streams       -lkobuki_dock_drive\
    -lecl_geometry    -lecl_threads \
    -lecl_io          -lecl_time

#jderobot
INCLUDEPATH +=  /usr/local/include/jderobot
LIBS += -L/usr/local/lib/jderobot \
        -lJderobotInterfaces

#ICE
LIBS += -L/usr/lib \
    -lIce -lIceUtil

OTHER_FILES += \
    config.cfg

