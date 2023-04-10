#ifndef STATEMENT_H
#define STATEMENT_H

#include <QList>
#include <QStringList>
#include "mainwindow.h"
#include "Dictionary.h"
#include "expression.h"


enum BasicCmd {
    NONE, REM, LET, PRINT, PRINTF, INPUT, INPUTS, GOTO, IF, END,
    RUN, DEBUG, STEP, LOAD, SAVE, CLEAR, HELP, QUIT, LIST, ERROR
};
BasicCmd strToCmd(const QString& s);


class Statement
{
public:
    Statement(const QString& s = "");
    Statement(const Statement& stat);
    Statement& operator=(const Statement& stat);
    virtual ~Statement();

    virtual QString check();
    virtual int exec(MainWindow* out, Dictionary<QString, Variable>* dic);
    bool isValid() const;
    virtual QString toString() const;
    virtual QString toTree() const;

private:
    QString raw;
    BasicCmd type;
    QString var;
    QChar op;
    QList<Expression*> exp;
    int jmp;
    bool parse_error;

    virtual void parse();
};

#endif // STATEMENT_H
