#ifndef FORMATSDIALOG_H
#define FORMATSDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/categorymodel/categorymodel.h"
#include "viewmodels/formatlistmodel/formatlistmodel.h"
#include <prefcontext.h>

namespace Ui {
class FormatsDialog;
}

class FormatsDialog : public QDialog
{
    Q_OBJECT
    
    public:
        explicit FormatsDialog(qint64 maxlen, qint64 pos, QWidget *parent = 0);
        FormatDefinition *selectedFormat();
        qint64 offset();
        ~FormatsDialog();

    private slots:
        void on_tvCategory_clicked(const QModelIndex &index);
        void on_tvFormats_doubleClicked(const QModelIndex &index);

    private:
        Ui::FormatsDialog *ui;
        FormatListModel* _formatsmodel;
        CategoryModel* _categorymodel;
};

#endif // FORMATSDIALOG_H
