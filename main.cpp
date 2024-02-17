#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	MainWindow window;
	
	QSettings settings("rpcsx", "rpcsx_ui_settings");
    settings.beginGroup("rpcsx_ui_settings");
	QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
		int windowHeight = window.height();
    	int windowWidth = window.width();
        settings.setValue("windowWidth", windowWidth);
        settings.setValue("windowHeight", windowHeight);
    });
    
	window.show();

    return app.exec();
}
