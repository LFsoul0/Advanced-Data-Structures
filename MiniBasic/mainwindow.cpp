#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadAction = ui->actionLoad;
    clear_loadAction = ui->actionClearAndLoad;
    saveAction = ui->actionSave;
    runAction = ui->actionRun;
    debugAction = ui->actionDebug;
    helpAction = ui->actionHelp;

    loadButton = ui->loadButton;
    runButton = ui->runButton;
    debugButton = ui->debugButton;
    clearButton = ui->clearButton;

    codeBrowser = ui->codeBrowser;
    resultBrowser = ui->resultBrowser;
    treeBrowser = ui->treeBrowser;
    varBrowser = ui->varBrowser;

    inputEdit = ui->inputEdit;

    connect(loadAction, &QAction::triggered, [=](){
        emit sendCommand("LOAD");
    });
    connect(clear_loadAction, &QAction::triggered, [=](){
        QStringList sl;
        sl.append("CLEAR");
        sl.append("LOAD");
        emit sendCommands(sl);
    });
    connect(saveAction, &QAction::triggered, [=](){
        emit sendCommand("SAVE");
    });
    connect(runAction, &QAction::triggered, [=](){
        emit sendCommand("RUN");
    });
    connect(debugAction, &QAction::triggered, [=](){
        emit sendCommand("DEBUG");
    });
    connect(helpAction, &QAction::triggered, [=](){
        emit sendCommand("HELP");
    });

    connect(loadButton, &QPushButton::clicked, [=](){
        emit sendCommand("LOAD");
    });
    connect(runButton, &QPushButton::clicked, [=](){
        emit sendCommand("RUN");
    });
    connect(debugButton, &QPushButton::clicked, [=](){
        emit sendCommand("DEBUG");
    });
    connect(clearButton, &QPushButton::clicked, [=](){
        emit sendCommand("CLEAR");
    });

    connect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::inputCommand);
}

void MainWindow::inputCommand()
{
    QString tmp = inputEdit->text();
    inputEdit->clear();
    emit sendCommands(tmp.trimmed().split('\n'));
}

void MainWindow::inputValue()
{
    inputEdit->setEnabled(false);
    disconnect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::inputValue);

    valStat = true;
    valSlot = inputEdit->text().toInt();

    inputEdit->setValidator(nullptr);
    inputEdit->setPlaceholderText("");
    inputEdit->clear();
    connect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::inputCommand);
}

void MainWindow::inputString()
{
    inputEdit->setEnabled(false);
    disconnect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::inputString);

    valStat = true;
    stringSlot = inputEdit->text();

    inputEdit->setValidator(nullptr);
    inputEdit->setPlaceholderText("");
    inputEdit->clear();
    connect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::inputCommand);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearCode()
{
    codeBrowser->clear();
}

void MainWindow::setCode(const QString &s, QList<QPair<int, QColor>>* highlights)
{
    codeBrowser->setText(s);
    if (highlights) {
        QTextCursor cursor(codeBrowser->document());
        QList<QTextEdit::ExtraSelection> extras;

        for (auto &line : *highlights) {
            QTextEdit::ExtraSelection h;
            h.cursor = cursor;
            h.cursor.setPosition(line.first);
            h.cursor.movePosition(QTextCursor::StartOfLine);
            h.cursor.movePosition(QTextCursor::EndOfLine);
            h.format.setProperty(QTextFormat::FullWidthSelection, true);
            h.format.setBackground(line.second);
            extras.append(h);
        }

        codeBrowser->setExtraSelections(extras);
    }
}

void MainWindow::appendCode(const QString& s)
{
    codeBrowser->append(s);
}

void MainWindow::clearResult()
{
    resultBrowser->clear();
}

void MainWindow::setResult(const QString &s)
{
    resultBrowser->setText(s);
}

void MainWindow::appendResult(const QString &s)
{
    resultBrowser->append(s);
}

void MainWindow::clearTree()
{
    treeBrowser->clear();
}

void MainWindow::setTree(const QString &s)
{
    treeBrowser->setText(s);
}

void MainWindow::appendTree(const QString &s)
{
    treeBrowser->append(s);
}

void MainWindow::clearVar()
{
    varBrowser->clear();
}

void MainWindow::setVar(const QString& s)
{
    varBrowser->setText(s);
}

void MainWindow::appendVar(const QString& s)
{
    varBrowser->append(s);
}

void MainWindow::setEditable(bool editable)
{
    inputEdit->setEnabled(editable);
    if (editable)
        inputEdit->setFocus();
}

int MainWindow::getInputValue(const QString& hint)
{
    disconnect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::inputCommand);

    inputEdit->setValidator(new QRegExpValidator(QRegExp("^-?\\d+$")));
    inputEdit->setPlaceholderText(hint);
    valStat = false;

    connect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::inputValue);
    inputEdit->setEnabled(true);
    inputEdit->setFocus();

    while(!QTest::qWaitFor([=](){ return valStat; }));

    return valSlot;
}

QString MainWindow::getInputString(const QString& hint)
{
    disconnect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::inputCommand);

    inputEdit->setValidator(new QRegExpValidator(QRegExp("^[^'\"]*$")));
    inputEdit->setPlaceholderText(hint);
    valStat = false;

    connect(inputEdit, &QLineEdit::returnPressed, this, &MainWindow::inputString);
    inputEdit->setEnabled(true);
    inputEdit->setFocus();

    while(!QTest::qWaitFor([=](){ return valStat; }));

    return stringSlot;
}

void MainWindow::runMask(bool start)
{
    static QStack<bool> s_load;
    static QStack<bool> s_run;
    static QStack<bool> s_debug;
    static QStack<bool> s_clear;

    if (start) {
        s_load.push(loadButton->isEnabled());
        s_run.push(runButton->isEnabled());
        s_debug.push(debugButton->isEnabled());
        s_clear.push(clearButton->isEnabled());

        loadButton->setEnabled(false);
        loadAction->setEnabled(false);
        runButton->setEnabled(false);
        runAction->setEnabled(false);
        debugButton->setEnabled(false);
        debugAction->setEnabled(false);
        clearButton->setEnabled(false);
        clear_loadAction->setEnabled(false);
    }
    else {
        if (s_load.empty()) s_load.push(true);
        if (s_run.empty()) s_run.push(true);
        if (s_debug.empty()) s_debug.push(true);
        if (s_clear.empty()) s_clear.push(true);

        bool pre_load = s_load.pop();
        bool pre_run = s_run.pop();
        bool pre_debug = s_debug.pop();
        bool pre_clear = s_clear.pop();

        loadButton->setEnabled(pre_load);
        loadAction->setEnabled(pre_load);
        runButton->setEnabled(pre_run);
        runAction->setEnabled(pre_run);
        debugButton->setEnabled(pre_debug);
        debugAction->setEnabled(pre_debug);
        clearButton->setEnabled(pre_clear);
        clear_loadAction->setEnabled(pre_load && pre_clear);
    }
}

void MainWindow::debugMask(bool start)
{
    static QStack<bool> s_load;
    static QStack<bool> s_clear;

    if (start) {
        s_load.push(loadButton->isEnabled());
        s_clear.push(clearButton->isEnabled());

        loadButton->setEnabled(false);
        loadAction->setEnabled(false);
        clearButton->setEnabled(false);
        clear_loadAction->setEnabled(false);
    }
    else {
        if (s_load.empty()) s_load.push(true);
        if (s_clear.empty()) s_clear.push(true);

        bool pre_load = s_load.pop();
        bool pre_clear = s_clear.pop();

        loadButton->setEnabled(pre_load);
        loadAction->setEnabled(pre_load);
        clearButton->setEnabled(pre_clear);
        clear_loadAction->setEnabled(pre_load && pre_clear);
    }
}

