#ifndef BINARYVIEWDIALOG_H
#define BINARYVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class BinaryViewDialog;
}

class BinaryViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BinaryViewDialog(QWidget *parent = 0);
    ~BinaryViewDialog();

private:
    Ui::BinaryViewDialog *ui;
};

#endif // BINARYVIEWDIALOG_H
