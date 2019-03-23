/********************************************************************************
** Form generated from reading UI file 'ModOrganizer.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODORGANIZER_H
#define UI_MODORGANIZER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModOrganizerClass
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QDial *dial;
    QLabel *label;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ModOrganizerClass)
    {
        if (ModOrganizerClass->objectName().isEmpty())
            ModOrganizerClass->setObjectName(QStringLiteral("ModOrganizerClass"));
        ModOrganizerClass->resize(600, 400);
        actionExit = new QAction(ModOrganizerClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(ModOrganizerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        dial = new QDial(centralWidget);
        dial->setObjectName(QStringLiteral("dial"));
        dial->setGeometry(QRect(70, 50, 50, 64));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(160, 70, 47, 13));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(300, 60, 75, 23));
        ModOrganizerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ModOrganizerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        ModOrganizerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ModOrganizerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ModOrganizerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ModOrganizerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ModOrganizerClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(ModOrganizerClass);
        QObject::connect(dial, SIGNAL(valueChanged(int)), label, SLOT(setNum(int)));

        QMetaObject::connectSlotsByName(ModOrganizerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ModOrganizerClass)
    {
        ModOrganizerClass->setWindowTitle(QApplication::translate("ModOrganizerClass", "ModOrganizer", nullptr));
        actionExit->setText(QApplication::translate("ModOrganizerClass", "Exit", nullptr));
        label->setText(QApplication::translate("ModOrganizerClass", "TextLabel", nullptr));
        pushButton->setText(QApplication::translate("ModOrganizerClass", "PushButton", nullptr));
        menuFile->setTitle(QApplication::translate("ModOrganizerClass", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModOrganizerClass: public Ui_ModOrganizerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODORGANIZER_H
