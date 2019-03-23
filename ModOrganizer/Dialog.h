#pragma once
#ifndef __Dialog_H_Included__
#define __Dialog_H_Included__ 

#include <QDialog>
#include "ui_Dialog.h"
#include "LogicI.h"

class Dialog : public QDialog {

	Q_OBJECT

public :
	Dialog(QWidget*, LogicI*);
	~Dialog();

private:
	Ui::Dialog uiDialog;
	LogicI *logicI;

public slots:
	void reject() override;

private slots:
	void setText(QString, OutputType);
	bool confirmation(QString);
	void close();

};

#endif