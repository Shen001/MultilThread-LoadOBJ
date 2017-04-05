#include "glmainwindow.h"
#include <QApplication>
#include <glapplication.h>
#include <qobject.h>

int main(int argc, char *argv[])
{
	GlApplication app(argc, argv);
	QLocale::setDefault(QLocale::C);
#if QT_VERSION >= 0x050100
	// Enable support for highres images (added in Qt 5.1, but off by default)
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
	QCoreApplication::setApplicationName(GlApplication::AppName() + "_" + GlApplication::AppVersion());

	GLMainWindow window;
	
	window.setWindowTitle(GlApplication::AppName() + "_" + GlApplication::AppVersion());
	window.showMaximized();
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));//πÿ±’”¶”√

	return app.exec();
}
