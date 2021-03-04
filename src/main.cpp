#include "mainwindow.h"
#include "version.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	auto version = QString("%1.%2").arg(APP_VER_FIRST).arg(APP_VER_SECOND);

	QString filename = QLocale::system().name();
	QTranslator translator(&a);
	if(translator.load(filename,"://lang/")) a.installTranslator(&translator);

	MainWindow w;

	w.setWindowTitle( "TCP DEV SERVER v" + version );
	w.setWindowIcon( QIcon( "://index.ico" ) );
	w.setMinimumSize( 640, 480 );

	w.show();

	return a.exec();
}
