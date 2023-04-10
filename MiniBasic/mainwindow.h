#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QPushButton>
#include <QTextBrowser>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QTest>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void clearCode();
    void setCode(const QString& s, QList<QPair<int, QColor>>* highlights = nullptr);
    void appendCode(const QString& s);
    void clearResult();
    void setResult(const QString& s);
    void appendResult(const QString& s);
    void clearTree();
    void setTree(const QString& s);
    void appendTree(const QString& s);
    void clearVar();
    void setVar(const QString& s);
    void appendVar(const QString& s);
    void setEditable(bool editable);
    int getInputValue(const QString& hint = "Please input an integer here...");
    QString getInputString(const QString& hint = "Please input an string here...");

    void runMask(bool start = true);
    void debugMask(bool start = true);

private:
    Ui::MainWindow *ui;

    QAction* loadAction;
    QAction* clear_loadAction;
    QAction* saveAction;
    QAction* runAction;
    QAction* debugAction;
    QAction* helpAction;

    QPushButton* loadButton;
    QPushButton* runButton;
    QPushButton* debugButton;
    QPushButton* clearButton;

    QTextBrowser* codeBrowser;
    QTextBrowser* resultBrowser;
    QTextBrowser* treeBrowser;
    QTextBrowser* varBrowser;

    QLineEdit* inputEdit;

    void inputCommand();
    void inputValue();
    void inputString();

    bool valStat;
    int valSlot;
    QString stringSlot;

signals:
    void sendCommand(QString s);
    void sendCommands(QStringList sl);

};
#endif // MAINWINDOW_H
