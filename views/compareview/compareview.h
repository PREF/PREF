#ifndef COMPAREVIEW_H
#define COMPAREVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "actionwidget/actiontoolbar.h"
#include "prefsdk/sdkmanager.h"
#include "views/abstractview.h"
#include "viewmodels/comparemodel/comparemodel.h"
#include "compareworker.h"

using namespace PrefSDK;

namespace Ui {
class CompareView;
}

class CompareView : public AbstractView
{
    Q_OBJECT

    public:
        explicit CompareView(QString leftfile, QString rightfile, const QString &viewname, QLabel* labelinfo, QWidget *parent = 0);
        ~CompareView();

    private slots:
        void onCompareWorkerFinished();
        void updateLeftInfo(qint64);
        void updateRightInfo(qint64);
        void highlightDifferences();
        void on_diffList_doubleClicked(const QModelIndex &index);

    private:
        void createToolbar(QHexEdit *hexedit, QWidget* tbcontainer, ActionWidget *actionwidget);

    public: /* Overriden Methods */
        virtual bool canSave() const;
        virtual void updateStatusBar();
        virtual void log(const QString&);
        virtual void logLine(const QString&, LogWidget::LogLevel = LogWidget::Nothing);

    private:
        Ui::CompareView *ui;
        CompareWorker _worker;
        CompareWorker::OffsetList _offsetlist;
        CompareWorker::DifferenceMap _differencemap;
        CompareModel* _comparemodel;
        QHexEditData* _lefthexeditdata;
        QHexEditData* _righthexeditdata;
        QColor _diffcolor;
};

#endif // COMPAREVIEW_H
