#ifndef BYTECOLORSWIDGET_H
#define BYTECOLORSWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/bytecolors.h"

using namespace PrefSDK;

namespace Ui {
class ByteColorsWidget;
}

class ByteColorsWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit ByteColorsWidget(QWidget *parent = 0);
        ~ByteColorsWidget();

    private:
        Ui::ByteColorsWidget *ui;
};

#endif // BYTECOLORSWIDGET_H
