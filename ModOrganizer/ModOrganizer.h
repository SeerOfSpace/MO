#pragma once
#ifndef __ModOrganizer_H_Included__
#define __ModOrganizer_H_Included__

#include <QMainWindow>
#include "ui_ModOrganizer.h"
#include "Dialog.h"
#include "LogicI.h"

class ModOrganizer : public QMainWindow
{
	Q_OBJECT

public:
	ModOrganizer(QWidget *parent = Q_NULLPTR);

private:
	Ui::ModOrganizerClass ui;
	Dialog *dialog;
	LogicI *logicI;
	std::string modPath = "C:\\Users\\Chrispy\\Desktop\\Test Mods";
	std::string gamePath = "D:\\Steam Games\\steamapps\\common\\Stardew Valley\\Content";
	std::string exePath = "D:\\Steam Games\\steamapps\\common\\Stardew Valley\\Stardew Valley.exe";
	std::vector<std::experimental::filesystem::path> mods;
	std::wstring processName = L"StardewModdingAPI.exe";
	bool exiting = false;
	bool running = false;
	void closeEvent(QCloseEvent*) override;

public slots:
	void dialogClosed();

private slots:
	void open();
	void quit();
	
signals:


};

#endif