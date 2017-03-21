#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "logwidget/logwidget.h"

namespace Ui {
class OutputWidget;
}

class OutputWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit OutputWidget(QWidget *parent = 0);
        LogWidget* logWidget();
        ~OutputWidget();

    private:
        Ui::OutputWidget *ui;
};

#endif // OUTPUTWIDGET_H
