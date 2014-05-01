cache()

QT += core widgets opengl

TARGET = 277starter3d
TEMPLATE = app

INCLUDEPATH += ./include

SOURCES += main.cpp \
    myglwidget.cpp \
    mymainwindow.cpp \
    glcamera.cpp \
    vertex.cpp \
    face.cpp \
    halfedge.cpp \
    mesh.cpp \
    nearlyequal.cpp \
    grid.cpp \
    deformer.cpp

HEADERS += \
    myglwidget.h \
    mymainwindow.h \
    glcamera.h \
    vertex.h \
    face.h \
    halfedge.h \
    mesh.h \
    nearlyequal.h \
    grid.h \
    deformer.h

FORMS += \
    mymainwindow.ui \

RESOURCES += \
    shaders.qrc

OTHER_FILES += \
    diffuse.frag.glsl \
    wire.frag.glsl \
    diff.frag.glsl \
    diff.vert.glsl \
    wire.vert.glsl \
    README.txt
