#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>
#include <QRegExpValidator>
#include <QStack>
#include "Dictionary.h"

struct Variable {
    enum VarType {
        INT, STRING, ERROR
    } type;
    struct ValContainer {
        int intVal;
        QString stringVal;
        ValContainer();
        ~ValContainer();
    } val;
    Variable(int _val);
    Variable(const QString& _val);
    Variable(const Variable& other);
    ~Variable();
    QString getTypeString();
    QString getValString();
};

class Expression;

enum ExpType {
    Constant, Identifier, Compound, String
};

Expression* strToExp(const QString& s);

bool isConstant(const QString& s);
bool isIdentifier(const QString& s);
bool isString(const QString& s);


class Expression
{
public:
    Expression(bool v = true);
    virtual ~Expression();

    static Dictionary<QString, Variable>* varDic;

    bool isValid() const;
    virtual int eval() const;
    virtual QString stringVal() const;
    virtual Variable::VarType valType() const;
    virtual ExpType type() const = 0;

    virtual QString toString() const = 0;

private:
    bool valid;
};

class StringExp: public Expression
{
public:
    StringExp(const QString& s);
    virtual ~StringExp();

    virtual QString stringVal() const;
    virtual ExpType type() const;
    virtual Variable::VarType valType() const;

    virtual QString toString() const;

private:
    QString value;
};


class ConstantExp: public Expression
{
public:
    ConstantExp(int val, bool flag = true);
    virtual ~ConstantExp();

    virtual int eval() const;
    virtual ExpType type() const;

    virtual QString toString() const;

private:
    int value;
};

class IdentifierExp: public Expression
{
public:
    IdentifierExp(const QString& s);
    virtual ~IdentifierExp();

    virtual int eval() const;
    virtual ExpType type() const;
    virtual QString stringVal() const;
    virtual Variable::VarType valType() const;

    virtual QString toString() const;

private:
    QString name;
};

class CompoundExp: public Expression
{
public:
    CompoundExp(QChar c, Expression* l, Expression* r);
    virtual ~CompoundExp();

    virtual int eval() const;
    virtual ExpType type() const;

    virtual QString toString() const;

private:
    QChar op;
    Expression* left, * right;
};

#endif // EXPRESSION_H
