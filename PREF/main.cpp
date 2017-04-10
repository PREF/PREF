#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("PREF 3.0");

    MainWindow w;
    w.show();

    return a.exec();
}
