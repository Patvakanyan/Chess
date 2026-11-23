#include "src/mainwindow.h"

#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
	QLoggingCategory::setFilterRules(QStringLiteral("qt.multimedia.ffmpeg.info=false"));

#ifdef Q_OS_LINUX
	// Keep VA-API errors visible without routine driver-probe messages.
	if (!qEnvironmentVariableIsSet("LIBVA_MESSAGING_LEVEL"))
		qputenv("LIBVA_MESSAGING_LEVEL", "1");
#endif

	QApplication app(argc, argv);

	MainWindow window;
	window.show();

	return app.exec();
}
