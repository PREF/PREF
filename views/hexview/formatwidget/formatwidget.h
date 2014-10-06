#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/format/formatlist.h"
#include "prefsdk/exporter/exporterlist.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "views/hexview/workertab.h"
#include "logwidget/logwidget.h"
#include "formattreeview/formattreeview.h"
#include "formatsdialog/formatsdialog.h"

using namespace PrefSDK;

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

    signals:
        void parseStarted();
        void parseFinished(FormatTree* formattree, QWidget* formatview);

    private:
        Ui::FormatWidget *ui;
        FormatModel* _formatmodel;
        QHexEdit* _hexedit;
        LogWidget* _logwidget;
        FormatDefinition* _formatdefinition;
};

#endif // FORMATWIDGET_H
