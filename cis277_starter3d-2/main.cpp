#include "mymainwindow.h"
#include "gui.h"
#include <QApplication>
#include <QGLFormat>
#include "glcamera.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGLFormat glFormat = QGLFormat::defaultFormat();
    glFormat.setVersion(3, 2);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);
    QGLFormat::setDefaultFormat(glFormat);
    QGLFormat::setDefaultFormat(glFormat);
    MyMainWindow w;
    w.show();

    return a.exec();
}

