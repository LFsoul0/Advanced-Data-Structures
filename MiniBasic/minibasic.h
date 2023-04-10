#ifndef MINIBASIC_H
#define MINIBASIC_H

#include <QObject>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>
#include <QDesktopServices>
#include <QApplication>
#include <QDebug>

#include "mainwindow.h"
#include "Skiplist.h"
#include "Dictionary.h"
#include "statement.h"

class MiniBasic : public QObject
{
    Q_OBJECT

public:
    explicit MiniBasic(QObject *parent = nullptr);
    ~MiniBasic();

    void loadCommand(const QString& s);
    void loadCommands(const QStringList& sl);

private:
    MainWindow* win;
    Skiplist<int, Statement>* codeList;
    Dictionary<QString, Variable>* varDic;

    QList<QPair<int, QColor>> highlights;

    bool debugMode;
    Node<int, Statement>* debugPointer;

    int execLine(Statement& s);
    void execProgram();

signals:

};

#endif // MINIBASIC_H
