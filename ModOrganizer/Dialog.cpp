#include "Dialog.h"
#include <QScrollBar>
#include <QMessageBox>

using namespace std;

Dialog::Dialog(QWidget * parent, LogicI* logicI)
	:QDialog(parent)
{
	uiDialog.setupUi(this);
	this->logicI = logicI;
	logicI->setQObject(this);
	qRegisterMetaType<OutputType>("OutputType");
	connect(logicI, SIGNAL(done()), this, SLOT(close()));
	connect(logicI, SIGNAL(done()), parent, SLOT(dialogClosed()));
	connect(logicI, SIGNAL(textReady(QString, OutputType)), this, SLOT(setText(QString, OutputType)));
	logicI->doWork();
}

Dialog::~Dialog() {
	//delete logicI;
}

void Dialog::setText(QString qstr, OutputType outputType) {
	switch (outputType) {
		case OutputType::Minor: uiDialog.textBrowser->setTextColor(Qt::yellow); break;
		case OutputType::Major: uiDialog.textBrowser->setTextColor(Qt::cyan); break;
		case OutputType::Error: uiDialog.textBrowser->setTextColor(Qt::red); break;
	}
	uiDialog.textBrowser->append(qstr);
	QScrollBar *bar = uiDialog.textBrowser->verticalScrollBar();
	bar->setValue(bar->maximum());
	//uiDialog.textBrowser->repaint();
}

bool Dialog::confirmation(QString qstr) {
	QMessageBox msgBox;
	msgBox.setText(qstr);
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Ok) {
		return true;
	}
	else {
		return false;
	}
}

void Dialog::close() {
	QDialog::reject();
}

void Dialog::reject() {
	logicI->stop();
}
