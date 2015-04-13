#ifndef COMPAREVIEW_H
#define COMPAREVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "actionwidget/actiontoolbar.h"
#include "views/abstractview.h"
#include "viewmodels/comparemodel/comparemodel.h"
#include "compareworker.h"

namespace Ui {
class CompareView;
}

class CompareView : public AbstractView
{
    Q_OBJECT

    public:
        explicit CompareView(QString leftfile, QString rightfile, QLabel* labelinfo, QWidget *parent = 0);
        virtual ~CompareView();

    private slots:
        void onCompareWorkerFinished();
        void updateLeftInfo(qint64);
        void updateRightInfo(qint64);
        void highlightDifferences();
        void on_diffList_doubleClicked(const QModelIndex &index);

    private:
        void createToolbar(QHexEdit *hexedit, QWidget* tbcontainer, ActionWidget *actionwidget);
        void highlightDiffBytes(qint64& offset);
        void highlightSameBytes(qint64& offset);

    public: /* Overriden Methods */
        virtual QHexEditData* data();
        virtual bool canSave() const;
        virtual void updateStatusBar();
        virtual LogWidget* logWidget();

    private:
        Ui::CompareView *ui;
        CompareWorker* _worker;
        CompareWorker::OffsetList _offsetlist;
        CompareWorker::DifferenceMap _differencemap;
        CompareModel* _comparemodel;
        QHexEditData* _lefthexeditdata;
        QHexEditData* _righthexeditdata;
        QHexEditDataReader* _leftreader;
        QHexEditDataReader* _rightreader;
        QColor _diffcolor;
        QColor _samecolor;
};

#endif // COMPAREVIEW_H
