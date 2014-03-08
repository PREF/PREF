#ifndef ACTIONWIDGET_H
#define ACTIONWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "byteopsaction/byteopsaction.h"
#include "findaction/findaction.h"
#include "gotoaction/gotoaction.h"
#include "rangeselectoraction/rangeselectoraction.h"

class ActionWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit ActionWidget(QWidget *parent = 0);
        FindAction* findAction();
        GotoAction* gotoAction();
        ByteOpsAction* byteOpsAction();
        RangeSelectorAction* rangeSelectorAction();

    private:
        void hideAllBottomItems(QWidget* ignore = nullptr);

    signals:
        void actionDisplayed();

    public slots:
        void showFind();
        void showGoto(qint64 max);
        void showByteOps();
        void showRangeSelector(qint64 max);
        void hideAllActions();

    private:
        QSpacerItem* _hspacer;
        QHBoxLayout* _hlayout;
        FindAction* _findaction;
        GotoAction* _gotoaction;
        ByteOpsAction* _byteopsaction;
        RangeSelectorAction* _rangeselectoraction;
};

#endif // ACTIONWIDGET_H
