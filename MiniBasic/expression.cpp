#include "expression.h"

Variable::ValContainer::ValContainer() { }

Variable::ValContainer::~ValContainer() { }

Variable::Variable(int _val)
{
    type = INT;
    val.intVal = _val;
}

Variable::Variable(const QString& _val)
{
    type = STRING;
    val.stringVal = _val;
}

Variable::Variable(const Variable& other)
{
    type = other.type;
    switch(type) {
    case INT:
        val.intVal = other.val.intVal;
        break;
    case STRING:
        val.stringVal = other.val.stringVal;
        break;
    case ERROR:
        break;
    }
}

Variable::~Variable()
{

}

QString Variable::getTypeString()
{
    switch (type) {
    case INT: return "INT";
    case STRING: return "STR";
    case ERROR: return "ERR";
    }
    return "ERR";
}

QString Variable::getValString()
{
    switch (type) {
    case INT: return QString::number(val.intVal);
    case STRING: return QString("\"") + val.stringVal + "\"";
    case ERROR: return "error";
    }
    return "error";
}


Dictionary<QString, Variable>* Expression::varDic = nullptr;

Expression* strToExp(const QString& s)
{
    QStack<QChar> ops;
    QStack<Expression*> exps;
    int p = 0;
    bool lastIsNum = false;
    bool end = false;

    QString trim = s.trimmed();
    if ((trim.front() == '"' && trim.back() == '"') || (trim.front() == '\'' && trim.back() == '\'')) {
        QString stringVal = trim.mid(1, trim.length() - 2);
        if (isString(stringVal))
            return new StringExp(stringVal);
    }

    ops.push('(');
    while (true) {
        if (lastIsNum) {
            if (ops.top() == '(') {
                while (p < s.length() && s[p].isSpace()) p++;
                if (p >= s.length()) {
                    ops.push(')');
                    end = true;
                    lastIsNum = false;
                }
                else if (s[p] == '+' || s[p] == '-' || s[p] == '*' || s[p] == '/' || s[p] == ')') {
                    ops.push(s[p]);
                    p++;
                    lastIsNum = false;
                }
                else {
                    return new ConstantExp(0, false);
                }
            }
            else if (ops.top() == '+' || ops.top() == '-'){
                while (p < s.length() && s[p].isSpace()) p++;
                if (p >= s.length()) {
                    QChar op = ops.pop();
                    Expression* right = exps.pop();
                    Expression* left = exps.pop();

                    exps.push(new CompoundExp(op, left, right));
                    ops.push(')');

                    end = true;
                    lastIsNum = false;
                }
                else if (s[p] == '*' || s[p] == '/') {
                    ops.push(s[p]);
                    p++;
                    lastIsNum = false;
                }
                else if (s[p] == '+' || s[p] == '-' || s[p] == ')') {
                    QChar op = ops.pop();
                    Expression* right = exps.pop();
                    Expression* left = exps.pop();

                    exps.push(new CompoundExp(op, left, right));
                    ops.push(s[p]);

                    p++;
                    lastIsNum = false;
                }
                else {
                    return new ConstantExp(0, false);
                }
            }
            else if (ops.top() == '*' || ops.top() == '/') {
                QChar op = ops.pop();
                Expression* right = exps.pop();
                Expression* left = exps.pop();

                exps.push(new CompoundExp(op, left, right));
                lastIsNum = true;
            }
            else {
                return new ConstantExp(0, false);
            }
        }
        else {
            if (ops.top() == ')') {
                ops.pop();
                if (ops.top() == '(') {
                    ops.pop();
                    lastIsNum = true;

                    if (ops.isEmpty()) {
                        if (end)
                            break;
                        else
                            return new ConstantExp(0, false);
                    }
                    else if (end) {
                        return new ConstantExp(0, false);
                    }
                }
                else {
                    return new ConstantExp(0, false);
                }
            }
            else {
                while (p < s.length() && s[p].isSpace()) p++;
                if (p >= s.length()) {
                    return new ConstantExp(0, false);
                }

                if (s[p] == '(') {
                    ops.push(s[p]);
                    p++;
                    lastIsNum = false;
                }
                else if (s[p] == '+' || s[p] == '-') {
                    if (ops.top() == '(') {
                        exps.push(new ConstantExp(0));
                        ops.push(s[p]);
                        p++;
                        lastIsNum = false;
                    }
                    else {
                        return new ConstantExp(0, false);
                    }
                }
                else {
                    QString val;
                    while (p < s.length() && !s[p].isSpace() && s[p] != '+' && s[p] != '-' && s[p] != '*' && s[p] != '/' && s[p] != ')') {
                        val.append(s[p++]);
                    }

                    if (isConstant(val)) {
                        exps.append(new ConstantExp(val.toInt()));
                        lastIsNum = true;
                    }
                    else if (isIdentifier(val)) {
                        exps.append(new IdentifierExp(val));
                        lastIsNum = true;
                    }
                    else {
                        return new ConstantExp(0, false);
                    }
                }
            }
        }
    }

    return exps.pop();
}

bool isConstant(const QString& s)
{
    QRegExp reg("^-?\\d+$");
    QRegExpValidator validator(reg);
    QString cpy = s;
    int pos = 0;
    bool acc = (QValidator::Acceptable == validator.validate(cpy, pos));
    return acc;
}

bool isIdentifier(const QString& s)
{
    QRegExp reg("^[A-Za-z_]\\w*$");
    QRegExpValidator validator(reg);
    QString cpy = s;
    int pos = 0;
    bool acc = (QValidator::Acceptable == validator.validate(cpy, pos));
    return acc;
}

bool isString(const QString& s)
{
    QRegExp reg("^[^'\"]*$");
    QRegExpValidator validator(reg);
    QString cpy = s;
    int pos = 0;
    bool acc = (QValidator::Acceptable == validator.validate(cpy, pos));
    return acc;
}



Expression::Expression(bool v)
{
    valid = v;
}

Expression::~Expression()
{

}

bool Expression::isValid() const
{
    return valid;
}

int Expression::eval() const
{
    return 0;
}

QString Expression::stringVal() const
{
    return QString("Error string expression");
}

Variable::VarType Expression::valType() const
{
    return Variable::INT;
}


StringExp::StringExp(const QString& s) : Expression()
{
    value = s;
}

StringExp::~StringExp()
{

}

QString StringExp::stringVal() const
{
    return value;
}

ExpType StringExp::type() const
{
    return String;
}

Variable::VarType StringExp::valType() const
{
    return Variable::STRING;
}

QString StringExp::toString() const
{
    return value;
}


ConstantExp::ConstantExp(int val, bool flag) : Expression(flag)
{
    value = val;
}

ConstantExp::~ConstantExp()
{

}

int ConstantExp::eval() const
{
    return value;
}

ExpType ConstantExp::type() const
{
    return Constant;
}

QString ConstantExp::toString() const
{
    return QString::number(value);
}

IdentifierExp::IdentifierExp(const QString& s) : Expression()
{
    name = s;
}

IdentifierExp::~IdentifierExp()
{

}

int IdentifierExp::eval() const
{
    Variable* val = varDic->get(name);
    if (!val) throw QString("Undefined variable ") + name + "\n";
    if (val->type != Variable::INT) throw QString("Variable ") + name + " is not an int \n";
    return val->val.intVal;
}

QString IdentifierExp::stringVal() const
{
    Variable* val = varDic->get(name);
    if (!val) throw QString("Undefined variable ") + name + "\n";
    switch(val->type) {
    case Variable::INT:
        return QString::number(val->val.intVal);
    case Variable::STRING:
        return val->val.stringVal;
    case Variable::ERROR:
        throw QString("Identifier type error\n");
    }
    throw QString("Identifier type error\n");
}

ExpType IdentifierExp::type() const
{
    return Identifier;
}

Variable::VarType IdentifierExp::valType() const
{
    Variable* val = varDic->get(name);
    if (!val) return Variable::ERROR;
    return val->type;
}

QString IdentifierExp::toString() const
{
    return name;
}

CompoundExp::CompoundExp(QChar c, Expression* l, Expression* r) : Expression()
{
    op = c;
    left = l;
    right = r;
}

CompoundExp::~CompoundExp()
{
    delete left;
    delete right;
}

int CompoundExp::eval() const
{
    if (op == '+')
        return left->eval() + right->eval();
    else if (op == '-')
        return left->eval() - right->eval();
    else if (op == '*')
        return left->eval() * right->eval();
    else if (op == '/') {
        if (right->eval() == 0)
            throw QString("Error: Division by 0!");
        return left->eval() / right->eval();
    }
    else
        throw QString("ERR: Exception in evaluation");
}

ExpType CompoundExp::type() const
{
    return Compound;
}

QString CompoundExp::toString() const
{
    QString lstr = left->toString().replace("\n", "\n    ");
    QString rstr = right->toString().replace("\n", "\n    ");

    return QString(op) + "\n    " + lstr + "\n    " + rstr;
}
