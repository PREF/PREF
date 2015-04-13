#ifndef ENTROPYWIDGET_H
#define ENTROPYWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <support/bytecolors.h>

using namespace PrefLib::Support;

class EntropyWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit EntropyWidget(QWidget *parent = 0);

    private:
        void adjust();

    protected:
        virtual void paintEvent(QPaintEvent*);
        virtual void resizeEvent(QResizeEvent*);

    private:
        qreal _step;
        qreal _stepwidth;
};

#endif // ENTROPYWIDGET_H
