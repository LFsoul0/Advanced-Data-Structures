#include "statement.h"

BasicCmd strToCmd(const QString& s)
{
    if (s == "REM") return REM;
    if (s == "LET") return LET;
    if (s == "PRINT") return PRINT;
    if (s == "PRINTF") return PRINTF;
    if (s == "INPUT") return INPUT;
    if (s == "INPUTS") return INPUTS;
    if (s == "GOTO") return GOTO;
    if (s == "IF") return IF;
    if (s == "END") return END;
    if (s == "RUN") return RUN;
    if (s == "DEBUG") return DEBUG;
    if (s == "STEP") return STEP;
    if (s == "LOAD") return LOAD;
    if (s == "SAVE") return SAVE;
    if (s == "CLEAR") return CLEAR;
    if (s == "HELP") return HELP;
    if (s == "QUIT") return QUIT;
    if (s == "LIST") return LIST;
    return ERROR;
}



Statement::Statement(const QString& s)
{
    raw = s;
    type = NONE;
    op = '\0';
    jmp = 0;
    parse_error = false;
}

Statement::Statement(const Statement& stat)
{
    raw = stat.raw;
    type = NONE;
    op = '\0';
    jmp = 0;
    parse_error = false;
}

Statement& Statement::operator=(const Statement& stat)
{
    raw = stat.raw;
    type = NONE;
    op = '\0';
    jmp = 0;
    parse_error = false;

    return *this;
}

Statement::~Statement()
{
    for (Expression*& p : exp) {
        delete p;
    }
}

void Statement::parse()
{
    QStringList sp = raw.split(' ');
    type = strToCmd(sp[0]);
    sp.pop_front();
    switch (type) {
    case REM:
    {
        break;
    }
    case LET:
    {
        QStringList tmp = sp.join(' ').split('=');
        var = tmp.first().trimmed();

        tmp.pop_front();
        exp.append(strToExp(tmp.join('=')));

        break;
    }
    case PRINT:
    {
        exp.append(strToExp(sp.join(' ')));
        break;
    }
    case PRINTF:
    {
        QString tmp = sp.join(' ').trimmed();
        if (tmp.back() == ',') {
            exp.append(new ConstantExp(0, false));
            break;
        }
        while (!tmp.isEmpty()) {
            if (tmp[0] == '\'') {
                QStringList tmpList = tmp.mid(1).split('\'');
                exp.append(strToExp(QString('\'') + tmpList.first() + '\''));
                tmpList.pop_front();
                tmp = tmpList.join('\'').trimmed();
                if (tmp.isEmpty()) break;
                if (tmp[0] != ',') {
                    exp.append(new ConstantExp(0, false));
                    break;
                }

                tmp = tmp.mid(1).trimmed();
            }
            else if (tmp[0] == '"') {
                QStringList tmpList = tmp.mid(1).split('"');
                exp.append(strToExp(QString('"') + tmpList.first() + '"'));
                tmpList.pop_front();
                tmp = tmpList.join('"').trimmed();
                if (tmp.isEmpty()) break;
                if (tmp[0] != ',') {
                    exp.append(new ConstantExp(0, false));
                    break;
                }

                tmp = tmp.mid(1).trimmed();
            }
            else {
                QStringList tmpList = tmp.split(',');
                exp.append(strToExp(tmpList.first()));
                tmpList.pop_front();
                tmp = tmpList.join(',').trimmed();
            }
        }
        break;
    }
    case INPUT:
    case INPUTS:
    {
        var = sp.join(' ').trimmed();
        break;
    }
    case GOTO:
    {
        bool flag = true;
        jmp = sp.join(' ').toInt(&flag);
        if (!flag) jmp = -1;

        break;
    }
    case IF:
    {
        QStringList tmp = sp.join(' ').split(" THEN ");
        if (tmp.first().contains('=')){
            op = '=';
            QStringList exps = tmp.first().split('=');
            exp.append(strToExp(exps.first()));
            exps.pop_front();
            exp.append(strToExp(exps.join('=')));
        }
        else if (tmp.first().contains('<')) {
            op = '<';
            QStringList exps = tmp.first().split('<');
            exp.append(strToExp(exps.first()));
            exps.pop_front();
            exp.append(strToExp(exps.join('<')));
        }
        else if (tmp.first().contains('>')) {
            op = '>';
            QStringList exps = tmp.first().split('>');
            exp.append(strToExp(exps.first()));
            exps.pop_front();
            exp.append(strToExp(exps.join('>')));
        }
        else {
            op = '\0';
        }

        tmp.pop_front();
        bool flag = true;
        jmp = tmp.join(" THEN ").toInt(&flag);
        if (!flag) jmp = -1;

        break;
    }
    case END:
    {
        break;
    }
    default:
    {
        break;
    }
    }
}

int Statement::exec(MainWindow* out, Dictionary<QString, Variable>* dic)
{
    if (type == NONE) {
        parse();
    }

    switch (type) {
    case REM:
    {
        return 0;
    }
    case LET:
    {
        if (!isIdentifier(var)) {
            parse_error = true;
            throw QString("ERROR: Variable name invalid!");
        }

        if (exp.isEmpty() || !exp[0]->isValid()) {
            parse_error = true;
            throw QString("ERROR: Expression invalid!");
        }

        switch (exp[0]->valType()) {
        case Variable::INT:
        {
            int intVal = exp[0]->eval();
            dic->put(var, intVal);
            break;
        }
        case Variable::STRING:
        {
            QString stringVal = exp[0]->stringVal();
            dic->put(var, stringVal);
            break;
        }
        default:
            throw QString("ERROR: Variable type error!");
        }

        return 0;
    }
    case PRINT:
    {
        if (exp.isEmpty() || !exp[0]->isValid()) {
            parse_error = true;
            throw QString("ERROR: Expression invalid!");
        }

        switch (exp[0]->valType()) {
        case Variable::INT:
        {
            int intVal = exp[0]->eval();
            out->appendResult(QString::number(intVal));
            break;
        }
        case Variable::STRING:
        {
            QString stringVal = exp[0]->stringVal();
            out->appendResult(stringVal);
            break;
        }
        default:
            throw QString("ERROR: Variable type error!");
        }

        return 0;
    }
    case PRINTF:
    {
        if (exp.isEmpty()) {
            parse_error = true;
            throw QString("ERROR: Expression invalid!");
        }
        for (Expression*& p : exp) {
            if (!p->isValid()) {
                parse_error = true;
                throw QString("ERROR: Expression invalid!");
            }
        }
        if (exp[0]->valType() != Variable::STRING) {
            parse_error = true;
            throw QString("ERROR: The first parameter of INPUTS must be a string!");
        }

        QString format = exp[0]->stringVal();
        int i = 0, len = exp.size();
        QList<int> idxs;
        while (++i < len) {
            int index = format.indexOf("{}");
            if (index == -1) {
                parse_error = true;
                throw QString("ERROR: Error parameter number of INPUTS!");
            }

            format.replace(index, 2, "  ");
            idxs.append(index);
        }

        if (format.indexOf("{}") != -1) {
            parse_error = true;
            throw QString("ERROR: Error parameter number of INPUTS!");
        }

        for (int i = len - 1; i > 0; --i) {
            switch (exp[i]->valType()) {
            case Variable::INT:
            {
                int intVal = exp[i]->eval();
                format.replace(idxs[i - 1], 2, QString::number(intVal));
                break;
            }
            case Variable::STRING:
            {
                QString stringVal = exp[i]->stringVal();
                format.replace(idxs[i - 1], 2, stringVal);
                break;
            }
            default:
                throw QString("ERROR: Variable type error!");
            }
        }

        out->appendResult(format);

        return 0;
    }
    case INPUT:
    {
        if (!isIdentifier(var)) {
            parse_error = true;
            throw QString("ERROR: Variable name invalid!");
        }

        out->appendResult("?");
        dic->put(var, out->getInputValue());
        return 0;
    }
    case INPUTS:
    {
        if (!isIdentifier(var)) {
            parse_error = true;
            throw QString("ERROR: Variable name invalid!");
        }

        out->appendResult("?");
        dic->put(var, out->getInputString());
        return 0;
    }
    case GOTO:
    {
        if (jmp <= 0) {
            parse_error = true;
            throw QString("ERROR: Line number must be positive!");
        }

        return jmp;
    }
    case IF:
    {
        if (op == '\0') {
            parse_error = true;
            throw QString("ERROR: Operator missing or invalid!");
        }

        if (exp.size() < 2 || !exp[0]->isValid() || !exp[1]->isValid()) {
            parse_error = true;
            throw QString("ERROR: Expression invalid!");
        }

        if (jmp <= 0) {
            parse_error = true;
            throw QString("ERROR: Line number must be positive!");
        }

        bool cc;
        int val1 = exp[0]->eval();
        int val2 = exp[1]->eval();
        if (op == '=')
            cc = (val1 == val2);
        else if (op == '<')
            cc = (val1 < val2);
        else if (op == '>')
            cc = (val1 > val2);
        else {
            parse_error = true;
            throw QString("ERROR: Operator error!");
        }

        if (cc)
           return jmp;
        else
           return 0;
    }
    case END:
    {
        return -1;
    }
    default:
    {
        parse_error = true;
        throw QString("ERROR: Wrong type of command!");
    }
    }
}

QString Statement::check()
{
    if (type == NONE) {
        parse();
    }

    switch (type) {
    case REM:
    {
        break;
    }
    case LET:
    {
        if (!isIdentifier(var)) {
            parse_error = true;
            return "ERROR: Variable name invalid!";
        }

        if (exp.isEmpty() || !exp[0]->isValid()) {
            parse_error = true;
            return "ERROR: Expression invalid!";
        }

        break;
    }
    case PRINT:
    {
        if (exp.isEmpty() || !exp[0]->isValid()) {
            parse_error = true;
            return "ERROR: Expression invalid!";
        }

        break;
    }
    case PRINTF:
    {
        if (exp.isEmpty()) {
            parse_error = true;
            return "ERROR: Expression invalid!";
        }
        for (Expression*& p : exp) {
            if (!p->isValid()) {
                parse_error = true;
                return "ERROR: Expression invalid!";
            }
        }
        if (exp[0]->valType() != Variable::STRING) {
            parse_error = true;
            return "ERROR: The first parameter of INPUTS must be a string!";
        }

        QString format = exp[0]->stringVal();
        int i = 0, len = exp.size();
        while (++i < len) {
            int index = format.indexOf("{}");
            if (index == -1) {
                parse_error = true;
                return "ERROR: Error parameter number of INPUTS!";
            }

            format.replace(index, 2, "");
        }

        if (format.indexOf("{}") != -1) {
            parse_error = true;
            return "ERROR: Error parameter number of INPUTS!";
        }

        break;
    }
    case INPUT:
    case INPUTS:
    {
        if (!isIdentifier(var)) {
            parse_error = true;
            return "ERROR: Variable name invalid!";
        }

        break;
    }
    case GOTO:
    {
        if (jmp <= 0) {
            parse_error = true;
            return "ERROR: Line number must be positive!";
        }

        break;
    }
    case IF:
    {
        if (op == '\0') {
            parse_error = true;
            return "ERROR: Operator missing or invalid!";
        }

        if (exp.size() < 2 || !exp[0]->isValid() || !exp[1]->isValid()) {
            parse_error = true;
            return "ERROR: Expression invalid!";
        }

        if (jmp <= 0) {
            parse_error = true;
            return "ERROR: Line number must be positive!";
        }

        break;
    }
    case END:
    {
        break;
    }
    default:
    {
        parse_error = true;
        return "ERROR: Wrong type of command!";
    }
    }

    return "";
}

bool Statement::isValid() const
{
    return !parse_error;
}

QString Statement::toString() const
{
    return raw;
}

QString Statement::toTree() const
{
    switch (type) {
    case REM:
    {
        return QString("REM\n    ") + raw.mid(4);
    }
    case LET:
    {
        return QString("LET = \n    ") + var + "\n    " + exp[0]->toString().replace("\n", "\n    ");
    }
    case PRINT:
    {
        return QString("PRINT\n    ") + exp[0]->toString().replace("\n", "\n    ");
    }
    case PRINTF:
    {
        QString ret = "PRINTF\n    ";
        for (Expression* p : exp) {
            ret += p->toString().replace("\n", "\n    ");
            ret += "\n    ";
        }
        return ret;
    }
    case INPUT:
    {
        return QString("INPUT\n    ") + var;
    }
    case INPUTS:
    {
        return QString("INPUTS\n    ") + var;
    }
    case GOTO:
    {
        return QString("GOTO\n    ") + QString::number(jmp);
    }
    case IF:
    {
        return QString("IF THEN\n    ") + exp[0]->toString().replace("\n", "\n    ") + "\n    " + op + "\n    " + exp[1]->toString().replace("\n", "\n    ") + "\n    " + QString::number(jmp);
    }
    case END:
    {
        return QString("END");
    }
    default:
    {
        return QString("This statement has never been parsed.");
    }
    }
}
