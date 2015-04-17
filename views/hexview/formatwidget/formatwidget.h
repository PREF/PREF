#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/formatmodel/formatmodel.h"
#include "views/hexview/workertab.h"
#include "prefsdk/qdatabuffer.h"
#include "logwidget/logwidget.h"
#include "formattreeview/formattreeview.h"
#include "formatsdialog/formatsdialog.h"
#include "formatworker.h"

namespace Ui {
class FormatWidget;
}

class FormatWidget : public WorkerTab
{
    Q_OBJECT

    public:
        explicit FormatWidget(QWidget *parent = 0);
        void setLogWidget(LogWidget* logwidget);
        void setData(QHexEdit *hexedit);
        QWidget* formatView();
        void resetData();
        ~FormatWidget();

    public slots:
        void loadFormat();

    private slots:
        void onSetBackColor(FormatElement *formatelement);
        void onRemoveBackColor(FormatElement *formatelement);
        void onFormatObjectSelected(FormatElement* formatelement);
        void exportData(FormatElement* formatelement);
        void importData(FormatElement *formatelement);
        void onParseCompleted();

    signals:
        void parsingStarted();
        void parsingCompleted();
        void parsingFailed();

    private:
        Ui::FormatWidget *ui;
        FormatWorker* _worker;
        FormatModel* _formatmodel;
        QHexEdit* _hexedit;
        LogWidget* _logwidget;
        QWidget* _formatview;
};

#endif // FORMATWIDGET_H
