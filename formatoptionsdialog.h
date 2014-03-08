#ifndef FORMATOPTIONSDIALOG_H
#define FORMATOPTIONSDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/format/formatdefinition.h"
#include "viewmodels/optionmodel/optionmodel.h"

using namespace PrefSDK;

namespace Ui {
class FormatOptionsDialog;
}

class FormatOptionsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit FormatOptionsDialog(FormatDefinition::Ptr formatdef, FormatModel* formatmodel, ByteBuffer *bytebuffer, QWidget *parent = 0);
        ~FormatOptionsDialog();

    private:
        void validateFields();

    private slots:
        void on_tvOptions_doubleClicked(const QModelIndex &index);
        void on_tvOptions_clicked(const QModelIndex &index);
        void on_pbExecute_clicked();

    private:
        Ui::FormatOptionsDialog *ui;
        FormatDefinition::Ptr _formatdefinition;
        FormatModel* _formatmodel;
        ByteBuffer* _bytebuffer;
        int _selindex;
};

#endif // FORMATOPTIONSDIALOG_H
