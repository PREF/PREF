#ifndef COMPAREDIALOG_H
#define COMPAREDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui {
class CompareDialog;
}

class CompareDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit CompareDialog(QWidget *parent = 0);
        ~CompareDialog();
        QString leftCompare() const;
        QString rightCompare() const;

    private:
        QString selectFile();

    private slots:
        void on_tbLeft_clicked();
        void on_tbRight_clicked();

    private:
        Ui::CompareDialog *ui;
        QString _left;
        QString _right;
};

#endif // COMPAREDIALOG_H
