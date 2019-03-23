#pragma once
#ifndef __LogicI_H_Included__
#define __LogicI_H_Included__ 

#include "Logic.h"
#include <QThread>

enum class OutputType { Minor, Major, Error };

class LogicI : public QThread, public MOLogic {

	Q_OBJECT

public:
	QObject *qobject;

	LogicI(MOLogicStruct moStruct)
		:MOLogic(moStruct) {}

	void setQObject(QObject* qobject) {
		this->qobject = qobject;
	}

	void doWork() {
		QThread::start();
	}

	void run() override {
		QThread::msleep(1000);
		MOLogic::start();
		QThread::msleep(1000);
		emit done();
	}

	virtual void minorOutput(std::string s) {
		QString qstr = QString::fromStdString(s);
		emit textReady(qstr, OutputType::Minor);
	}

	virtual void majorOutput(std::string s) {
		QString qstr = QString::fromStdString(s);
		emit textReady(qstr, OutputType::Major);
	}

	virtual void errorOutput(std::string s) {
		QString qstr = QString::fromStdString(s);
		emit textReady(qstr, OutputType::Error);
	}

	virtual bool confirmation(std::string s) {
		bool confirm = true;
		QString qstr = QString::fromStdString(s);
		QMetaObject::invokeMethod(qobject, "confirmation", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, confirm), Q_ARG(QString, qstr));
		return confirm;
	}

signals:
	void textReady(QString, OutputType);
	void done();

};

#endif