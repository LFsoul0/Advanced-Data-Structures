#include "minibasic.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MiniBasic b;
    return a.exec();
}
