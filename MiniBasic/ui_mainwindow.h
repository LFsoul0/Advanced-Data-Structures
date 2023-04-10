/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad;
    QAction *actionClearAndLoad;
    QAction *actionHelp;
    QAction *actionRun;
    QAction *actionSave;
    QAction *actionDebug;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QTextBrowser *codeBrowser;
    QLabel *labelResult;
    QLabel *labelTree;
    QLabel *labelCode;
    QTextBrowser *resultBrowser;
    QTextBrowser *treeBrowser;
    QLabel *labelVar;
    QTextBrowser *varBrowser;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *loadButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *runButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *debugButton;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *clearButton;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelInput;
    QLineEdit *inputEdit;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuExec;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1031, 840);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon.png"), QSize(), QIcon::Normal, QIcon::On);
        MainWindow->setWindowIcon(icon);
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionClearAndLoad = new QAction(MainWindow);
        actionClearAndLoad->setObjectName(QString::fromUtf8("actionClearAndLoad"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionDebug = new QAction(MainWindow);
        actionDebug->setObjectName(QString::fromUtf8("actionDebug"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(7, 7, 7, 7);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        codeBrowser = new QTextBrowser(centralwidget);
        codeBrowser->setObjectName(QString::fromUtf8("codeBrowser"));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        codeBrowser->setFont(font);

        gridLayout->addWidget(codeBrowser, 1, 0, 1, 1);

        labelResult = new QLabel(centralwidget);
        labelResult->setObjectName(QString::fromUtf8("labelResult"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setBold(false);
        font1.setWeight(50);
        labelResult->setFont(font1);

        gridLayout->addWidget(labelResult, 0, 1, 1, 1);

        labelTree = new QLabel(centralwidget);
        labelTree->setObjectName(QString::fromUtf8("labelTree"));
        labelTree->setFont(font1);

        gridLayout->addWidget(labelTree, 2, 0, 1, 1);

        labelCode = new QLabel(centralwidget);
        labelCode->setObjectName(QString::fromUtf8("labelCode"));
        labelCode->setFont(font1);

        gridLayout->addWidget(labelCode, 0, 0, 1, 1);

        resultBrowser = new QTextBrowser(centralwidget);
        resultBrowser->setObjectName(QString::fromUtf8("resultBrowser"));
        resultBrowser->setFont(font);

        gridLayout->addWidget(resultBrowser, 1, 1, 1, 1);

        treeBrowser = new QTextBrowser(centralwidget);
        treeBrowser->setObjectName(QString::fromUtf8("treeBrowser"));
        treeBrowser->setFont(font);

        gridLayout->addWidget(treeBrowser, 3, 0, 1, 1);

        labelVar = new QLabel(centralwidget);
        labelVar->setObjectName(QString::fromUtf8("labelVar"));
        labelVar->setFont(font);

        gridLayout->addWidget(labelVar, 2, 1, 1, 1);

        varBrowser = new QTextBrowser(centralwidget);
        varBrowser->setObjectName(QString::fromUtf8("varBrowser"));
        varBrowser->setFont(font);

        gridLayout->addWidget(varBrowser, 3, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(20, 12, 20, 12);
        loadButton = new QPushButton(centralwidget);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));
        loadButton->setMinimumSize(QSize(0, 0));
        loadButton->setFont(font1);
        loadButton->setAutoFillBackground(false);
        loadButton->setAutoDefault(false);
        loadButton->setFlat(false);

        horizontalLayout_2->addWidget(loadButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        runButton = new QPushButton(centralwidget);
        runButton->setObjectName(QString::fromUtf8("runButton"));
        runButton->setMinimumSize(QSize(0, 0));
        runButton->setFont(font1);

        horizontalLayout_2->addWidget(runButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        debugButton = new QPushButton(centralwidget);
        debugButton->setObjectName(QString::fromUtf8("debugButton"));
        debugButton->setMinimumSize(QSize(0, 0));
        debugButton->setFont(font1);

        horizontalLayout_2->addWidget(debugButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        clearButton = new QPushButton(centralwidget);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setFont(font);

        horizontalLayout_2->addWidget(clearButton);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(2, 1);
        horizontalLayout_2->setStretch(4, 1);
        horizontalLayout_2->setStretch(6, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        labelInput = new QLabel(centralwidget);
        labelInput->setObjectName(QString::fromUtf8("labelInput"));
        labelInput->setFont(font1);

        verticalLayout_3->addWidget(labelInput);

        inputEdit = new QLineEdit(centralwidget);
        inputEdit->setObjectName(QString::fromUtf8("inputEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inputEdit->sizePolicy().hasHeightForWidth());
        inputEdit->setSizePolicy(sizePolicy1);
        inputEdit->setMinimumSize(QSize(0, 0));
        inputEdit->setFont(font);
        inputEdit->setCursor(QCursor(Qt::IBeamCursor));

        verticalLayout_3->addWidget(inputEdit);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 2);

        verticalLayout->addLayout(verticalLayout_3);

        verticalLayout->setStretch(0, 3);

        verticalLayout_4->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1031, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuExec = new QMenu(menubar);
        menuExec->setObjectName(QString::fromUtf8("menuExec"));
        MainWindow->setMenuBar(menubar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuExec->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionClearAndLoad);
        menuFile->addAction(actionSave);
        menuHelp->addAction(actionHelp);
        menuExec->addAction(actionRun);
        menuExec->addAction(actionDebug);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MiniBasic", nullptr));
        actionLoad->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
#if QT_CONFIG(tooltip)
        actionLoad->setToolTip(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
#endif // QT_CONFIG(tooltip)
        actionClearAndLoad->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\271\266\346\211\223\345\274\200", nullptr));
#if QT_CONFIG(tooltip)
        actionClearAndLoad->setToolTip(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\271\266\346\211\223\345\274\200", nullptr));
#endif // QT_CONFIG(tooltip)
        actionHelp->setText(QCoreApplication::translate("MainWindow", "\344\275\277\347\224\250\346\214\207\345\215\227", nullptr));
        actionRun->setText(QCoreApplication::translate("MainWindow", "\346\211\247\350\241\214", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
#if QT_CONFIG(tooltip)
        actionSave->setToolTip(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
#endif // QT_CONFIG(tooltip)
        actionDebug->setText(QCoreApplication::translate("MainWindow", "\350\260\203\350\257\225/\345\215\225\346\255\245", nullptr));
#if QT_CONFIG(tooltip)
        actionDebug->setToolTip(QCoreApplication::translate("MainWindow", "\350\260\203\350\257\225/\345\215\225\346\255\245", nullptr));
#endif // QT_CONFIG(tooltip)
        labelResult->setText(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\347\273\223\346\236\234", nullptr));
        labelTree->setText(QCoreApplication::translate("MainWindow", "\350\257\255\345\217\245\344\270\216\350\257\255\346\263\225\346\240\221", nullptr));
        labelCode->setText(QCoreApplication::translate("MainWindow", "\344\273\243\347\240\201", nullptr));
        labelVar->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\345\217\230\351\207\217", nullptr));
        loadButton->setText(QCoreApplication::translate("MainWindow", "\350\275\275\345\205\245\344\273\243\347\240\201\357\274\210LOAD\357\274\211", nullptr));
        runButton->setText(QCoreApplication::translate("MainWindow", "\346\211\247\350\241\214\344\273\243\347\240\201\357\274\210RUN\357\274\211", nullptr));
        debugButton->setText(QCoreApplication::translate("MainWindow", "\350\260\203\350\257\225/\345\215\225\346\255\245\357\274\210DEBUG/STEP\357\274\211", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\344\273\243\347\240\201\357\274\210CLAER\357\274\211", nullptr));
        labelInput->setText(QCoreApplication::translate("MainWindow", "\345\221\275\344\273\244\350\276\223\345\205\245\347\252\227\345\217\243", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
        menuExec->setTitle(QCoreApplication::translate("MainWindow", "\350\260\203\350\257\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
