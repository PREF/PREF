#ifndef FORMATSDIALOG_H
#define FORMATSDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/format/formatlist.h"
#include "viewmodels/categorymodel/categorymodel.h"
#include "viewmodels/formatlistmodel/formatlistmodel.h"

namespace Ui {
class FormatsDialog;
}

class FormatsDialog : public QDialog
{
    Q_OBJECT
    
    public:
        explicit FormatsDialog(qint64 maxlen, QWidget *parent = 0);
        FormatList::FormatId selectedFormat();
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
