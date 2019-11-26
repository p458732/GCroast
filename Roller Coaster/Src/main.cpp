#include "AppMain.h"
#include <QtWidgets/QApplication>
#include <QtMultimedia/QMediaPlayer>

int main(int argc, char *argv[])
{
	QMediaPlayer* player;

	player = new QMediaPlayer;
	player->setMedia(QUrl(QUrl::fromLocalFile("music.mp3")));
	player->setVolume(50);
	player->play();
	QApplication a(argc, argv);
	//test
	AppMain *window = AppMain::getInstance();
	window->show();
	return a.exec();
}
