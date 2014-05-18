#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/sdkmanager.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/exporter/exporterlist.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "views/hexview/workertab.h"
#include "formattreeview/formattreeview.h"
#include "formatsdialog.h"

using namespace PrefSDK;

namespace Ui {
class FormatWidget;
}

class FormatWidget : public WorkerTab
{
    Q_OBJECT

    public:
        explicit FormatWidget(QWidget *parent = 0);
        FormatTree* tree();
        void setData(QHexEdit *hexedit);
        ~FormatWidget();

    private:
        FormatTree* parseFormat(FormatList::FormatId formatid, qint64 baseoffset);

    private slots:
        void onSetBackColor(FormatElement *formatelement);
        void onRemoveBackColor(FormatElement *formatelement);
        void onFormatObjectSelected(FormatElement* formatelement);
        void exportData(FormatElement* formatelement);
        void importData(FormatElement *formatelement);

    public slots:
        FormatList::FormatId loadFormat();

    private:
        Ui::FormatWidget *ui;
        FormatModel* _formatmodel;
        QHexEdit* _hexedit;
        FormatTree* _formattree;
};

#endif // FORMATWIDGET_H
