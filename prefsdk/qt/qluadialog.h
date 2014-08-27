#ifndef QLUADIALOG_H
#define QLUADIALOG_H

#include <QDialog>

/*
 * QDialog with enhanced Signal/Slots support
 */

class QLuaDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit QLuaDialog(QWidget *parent = 0);

    public slots:
        void setPosition(int x, int y);
        void setSize(int w, int h);
};

#endif // QLUADIALOG_H
