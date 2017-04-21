#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName(QString("PREF 3.0-") + GIT_VERSION);

    MainWindow w;
    w.show();

    return a.exec();
}
