#ifndef STRINGSWIDGET_H
#define STRINGSWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "stringsworker.h"
#include "viewmodels/stringsmodel/stringsmodel.h"

namespace Ui {
class StringsWidget;
}

class StringsWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit StringsWidget(QWidget *parent = 0);
        void scan(QHexEditData* hexeditdata);
        ~StringsWidget();

    private:
        void createStringListActions();
        void displayNotFoundMessage();
        QModelIndex search(StringsModel::SearchDirection direction);

    private slots:
        void onStringsWorkerFinished();
        void onCopyTriggered();
        void onCopyOffsetTriggered();
        void onCopyStringTriggered();
        void on_stringList_customContextMenuRequested(const QPoint &pos);
        void on_stringList_doubleClicked(const QModelIndex &index);
        void on_tbSearchUp_clicked();
        void on_tbSearchDown_clicked();

    signals:
        void gotoTriggered(qint64 offset, qint64 length);

    private:
        Ui::StringsWidget *ui;
        StringsWorker _worker;
        QHexEditData* _hexeditdata;
        StringsModel* _stringsmodel;
        QMenu* _slmenu;
        QAction* _actgoto;
        QAction* _actcopy;
        QAction* _actcopyoffset;
        QAction* _actcopystring;
        QAction* _actexportcsv;
};

#endif // STRINGSWIDGET_H
