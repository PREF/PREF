#ifndef DISPLAYMODEWIDGET_H
#define DISPLAYMODEWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "binarymapwidget/binarymap.h"

namespace Ui {
class DisplayModeWidget;
}

class DisplayModeWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit DisplayModeWidget(QWidget *parent = 0);
        ~DisplayModeWidget();

    signals:
        void displayModeChanged(BinaryMap::DisplayMode mode);

    private slots:
        void on_comboBox_currentIndexChanged(int index);

    private:
        Ui::DisplayModeWidget *ui;
};

#endif // DISPLAYMODEWIDGET_H
