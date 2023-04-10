#include "minibasic.h"

MiniBasic::MiniBasic(QObject *parent) : QObject(parent)
{
    win = new MainWindow();
    win->show();

    codeList = new Skiplist<int, Statement>();
    varDic = new Dictionary<QString, Variable>();

    connect(win, &MainWindow::sendCommand, this, &MiniBasic::loadCommand);
    connect(win, &MainWindow::sendCommands, this, &MiniBasic::loadCommands);

    Expression::varDic = this->varDic;

    debugMode = false;
    debugPointer = nullptr;
}

MiniBasic::~MiniBasic()
{
    delete win;
    delete codeList;
    delete varDic;
}

void MiniBasic::loadCommand(const QString& s)
{
    if (s.trimmed().isEmpty()) return;

    QStringList tmp = s.trimmed().split(' ');
    bool numed = true;
    int key = tmp[0].toInt(&numed);

    if (numed){
        if (debugMode) return;
        if (key <= 0) {
            QMessageBox::warning(win, "Error", "Line number must be positive!");
            return;
        }
        tmp.pop_front();
        if (tmp.isEmpty()) {
            codeList->remove(key);
        }
        else {
            codeList->put(key, tmp.join(' ').trimmed());
        }
        highlights.clear();
        QString codeString = codeList->toString(&highlights);
        win->setCode(codeString, &highlights);
        return;
    }

    switch (strToCmd(tmp[0])) {
    case RUN:
    {
        if (debugMode) {
            while (debugMode) {
                loadCommand("STEP");
            }
            return;
        }
        execProgram();
        return;
    }
    case DEBUG:
    case STEP:
    {
        if (!debugMode) {
            debugMode = true;
            win->debugMask(true);
            debugPointer = codeList->getHead();
        }
        else {
            win->setEditable(false);
            win->runMask(true);

            int jmp = execLine(debugPointer->val);

            if (jmp == 0 && debugPointer->right) {
                debugPointer = debugPointer->right;
            }
            else if (jmp > 0) {
                debugPointer = codeList->get(jmp);
                if (!debugPointer) {
                    QMessageBox::warning(win, "Error", QString("Line %1 doesn't exist!").arg(jmp));
                }
            }
            else {
                QMessageBox::information(win, "Program End", QString("Program end at line %1.").arg(debugPointer->key));
                debugPointer = nullptr;
            }

            win->runMask(false);
            win->setEditable(true);
        }
        if (debugPointer) {
            QString tr = debugPointer->val.check();
            if (tr != "") {
                win->setTree(QString::number(debugPointer->key) + " " + tr);
            }
            else {
                win->setTree(QString::number(debugPointer->key) + " " + debugPointer->val.toTree());
            }
        }
        else {
            debugMode = false;
            win->debugMask(false);
            win->clearTree();
        }
        highlights.clear();
        QString codeString = codeList->toString(&highlights, debugPointer);
        win->setCode(codeString, &highlights);

        return;
    }
    case LOAD:
    {
        if (debugMode) return;
        QString filename = QFileDialog::getOpenFileName(win);
        if (filename == "") return;
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(win, "Error", "Failed to open file!");
            return;
        }
        QTextStream in(&file);
        QStringList sl;
        while (!in.atEnd()) {
            sl.append(in.readLine());
        }
        file.close();
        loadCommands(sl);
        return;
    }
    case SAVE:
    {
        QString filename = QFileDialog::getSaveFileName(win);
        if (filename == "") return;
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(win, "Error", "Failed to open file!");
            return;
        }
        QTextStream out(&file);
        out << codeList->toString();
        file.close();
        return;
    }
    case CLEAR:
    {
        if (debugMode) return;
        codeList->clear();
        varDic->clear();
        win->clearCode();
        win->clearResult();
        win->clearTree();
        win->clearVar();
        highlights.clear();
        return;
    }
    case HELP:
    {
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath().append("/Basic-doc.pdf")))) {
            QFile helpfile("://Basic-doc.pdf");
            helpfile.copy(QApplication::applicationDirPath().append("/Basic-doc.pdf"));
            if (!QDesktopServices::openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath().append("/Basic-doc.pdf")))) {
                QMessageBox::warning(win, "Error", "Help document missing!");
                return;
            }
        }
        return;
    }
    case QUIT:
    {
        win->close();
        return;
    }
    case LET:
    case INPUT:
    case INPUTS:
    case PRINT:
    case PRINTF:
    {
        Statement stat(tmp.join(' '));
        win->setEditable(false);
        win->runMask(true);
        execLine(stat);
        win->runMask(false);
        win->setEditable(true);
        return;
    }
    case LIST:
    {
        return;
    }
    default:
        QMessageBox::warning(win, "Error", "Wrong type of command!");
        return;
    }
}

void MiniBasic::loadCommands(const QStringList& sl)
{
    for (QString s : sl) {
        loadCommand(s);
    }
}

int MiniBasic::execLine(Statement& s)
{
    int ret = -1;
    try {
        ret = s.exec(win, varDic);
        win->setVar(varDic->toString());
    }
    catch(QString& s) {
        QMessageBox::warning(win, "Error", s);
    }
    catch(...) {
        QMessageBox::warning(win, "Error", "UNKNOWN ERROR!");
    }
    return ret;
}

void MiniBasic::execProgram()
{
    win->setEditable(false);
    win->runMask(true);

    win->clearTree();
    auto p = codeList->getHead();
    while (p) {
        QString tr = p->val.check();
        if (tr != "") {
            win->appendTree(QString::number(p->key) + " " + tr);
        }
        else {
            win->appendTree(QString::number(p->key) + " " + p->val.toTree());
        }
        p = p->right;
    }
    highlights.clear();
    QString codeString = codeList->toString(&highlights);
    win->setCode(codeString, &highlights);

    p = codeList->getHead();
    int jmp;
    while(p) {
        jmp = execLine(p->val);

        if (jmp == 0 && p->right) {
            p = p->right;
        }
        else if (jmp > 0) {
            p = codeList->get(jmp);
            if (!p) {
                QMessageBox::warning(win, "Error", QString("Line %1 doesn't exist!").arg(jmp));
                break;
            }
        }
        else {
            QMessageBox::information(win, "Program End", QString("Program end at line %1.").arg(p->key));
            break;
        }
    }

    win->runMask(false);
    win->setEditable(true);
}
