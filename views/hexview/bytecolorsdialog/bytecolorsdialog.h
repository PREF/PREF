#ifndef BYTECOLORSWIDGET_H
#define BYTECOLORSWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/bytecolors.h"

using namespace PrefSDK;

namespace Ui {
class ByteColorsDialog;
}

class ByteColorsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit ByteColorsDialog(QWidget *parent = 0);
        ~ByteColorsDialog();

    private:
        Ui::ByteColorsDialog *ui;
};

#endif // BYTECOLORSWIDGET_H
