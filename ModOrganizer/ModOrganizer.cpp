#include "ModOrganizer.h"
#include <QCloseEvent>

using namespace std;

ModOrganizer::ModOrganizer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.menuFile, SIGNAL(triggered(QAction*)), this, SLOT(quit()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(open()));
}

void ModOrganizer::quit() {
	if (running) {
		exiting = true;
		dialog->reject();
	}
	else {
		QApplication::quit();
	}
}

void ModOrganizer::closeEvent(QCloseEvent *event) {
	if (running) {
		exiting = true;
		event->ignore();
		dialog->reject();
	}
	else {
		event->accept();
	}
}

void ModOrganizer::dialogClosed() {
	running = false;
	delete logicI;
	delete dialog;
	if (exiting) {
		QApplication::quit();
	}
}

void ModOrganizer::open() {
	if (!running) {
		running = true;
		unsigned int appID = 413150;
		mods.push_back("C:\\Users\\Chrispy\\Desktop\\Test Mods\\Mod1");
		MOLogicStruct moStruct;
		moStruct.modPath = modPath;
		moStruct.gamePath = gamePath;
		moStruct.appID = appID;
		moStruct.processName = processName;
		moStruct.mods = mods;
		logicI = new LogicI(moStruct);
		dialog = new Dialog(this, logicI);
		dialog->setAttribute(Qt::WA_DeleteOnClose);
		dialog->show();
	}
}