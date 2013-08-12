
SOURCES += \
    main.cpp \
    gui/gui.cpp \
    gui/threadgui.cpp \
    sensors/sensors.cpp \
    sensors/threadsensors.cpp
HEADERS+= \
    gui/gui.h \
    gui/threadgui.h \
    sensors/sensors.h \
    sensors/threadsensors.h

OTHER_FILES += \
    simple.cfg


#ICE
LIBS += -L/usr/lib \
    -lIce -lIceUtil

#opencv
INCLUDEPATH += /usr/include/opencv
LIBS += -L/usr/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc

#jderobot
INCLUDEPATH += /usr/local/include/jderobot
LIBS += -L/usr/local/lib/jderobot \
        -lbgfgsegmentation  -ljderobotice         -lprogeo \
        -lJderobotInterfaces  -lvisionlib \
        -lcolorspaces       -ljderobotutil


#OpenGL
INCLUDEPATH += /usr/include/GL
LIBS += -L/usr/lib \
    -lglut -lGL -lGLU
QT           += opengl
