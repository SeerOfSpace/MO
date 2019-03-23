#include "ModOrganizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ModOrganizer w;
	w.show();
	return a.exec();
}
