#ifndef SIGNATURESWIDGET_H
#define SIGNATURESWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "views/hexview/workertab.h"
#include "viewmodels/signaturesmodel/signaturesmodel.h"
#include "signaturesworker.h"

namespace Ui {
class SignaturesWidget;
}

class SignaturesWidget : public WorkerTab
{
    Q_OBJECT

    public:
        explicit SignaturesWidget(QWidget *parent = 0);
        void scan(QHexEditData* hexeditdata);
        ~SignaturesWidget();

    private:
        void createSignatureListMenu();

    private slots:
        void onSignaturesWorkerFinished();
        void onCopyTriggered();
        void onCopyStringTriggered();
        void onCopyOffsetTriggered();
        void on_signaturesTree_doubleClicked(const QModelIndex &index);
        void on_signaturesTree_customContextMenuRequested(const QPoint &pos);

    signals:
        void gotoTriggered(qint64 offset, qint64 length);

    private:
        Ui::SignaturesWidget *ui;
        QHexEditData* _hexeditdata;
        SignaturesWorker _worker;
        SignaturesModel* _signaturesmodel;
        QMenu* _slmenu;
        QAction* _actgoto;
        QAction* _actcopy;
        QAction* _actcopyoffset;
        QAction* _actcopystring;
        QAction* _actexportcsv;
};

#endif // SIGNATURESWIDGET_H
