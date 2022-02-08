#include "SimpleQtOpenGLApp.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
#if 1
	QSurfaceFormat format;
	format.setMajorVersion(3);
	format.setMinorVersion(3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);
#endif

    QApplication a(argc, argv);
    SimpleQtOpenGLApp w;
    w.show();
    return a.exec();
}
