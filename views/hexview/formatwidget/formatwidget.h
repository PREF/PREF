#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/format/formatlist.h"
#include "prefsdk/exporter/exporterlist.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "views/hexview/workertab.h"
#include "formattreeview/formattreeview.h"
#include "formatsdialog/formatsdialog.h"
#include "formatworker.h"

using namespace PrefSDK;

namespace Ui {
class FormatWidget;
}

class FormatWidget : public WorkerTab
{
    Q_OBJECT

    public:
        explicit FormatWidget(QWidget *parent = 0);
        void setData(QHexEdit *hexedit);
        ~FormatWidget();

    public slots:
        void loadFormat();

    private:
        FormatTree* parseFormat(FormatList::FormatId formatid, qint64 baseoffset);

    private slots:
        void onParsingFinished();
        void onSetBackColor(FormatElement *formatelement);
        void onRemoveBackColor(FormatElement *formatelement);
        void onFormatObjectSelected(FormatElement* formatelement);
        void exportData(FormatElement* formatelement);
        void importData(FormatElement *formatelement);

    signals:
        void parseStarted();
        void parseFinished(FormatList::FormatId formatid, FormatTree* formattree);

    private:
        Ui::FormatWidget *ui;
        FormatWorker _worker;
        FormatModel* _formatmodel;
        QHexEdit* _hexedit;
        FormatList::FormatId _formatid;
};

#endif // FORMATWIDGET_H
