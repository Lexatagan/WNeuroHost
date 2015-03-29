#include <QCoreApplication>
#include "interface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Interface webtouart;

    return a.exec();
}
