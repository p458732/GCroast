#include "AppMain.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//test
	AppMain *window = AppMain::getInstance();
	window->show();
	return a.exec();
}
