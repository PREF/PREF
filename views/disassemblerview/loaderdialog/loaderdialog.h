#ifndef LOADERDIALOG_H
#define LOADERDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/loader/loaderlist.h"
#include "viewmodels/loaderlistmodel/loaderlistmodel.h"

using namespace PrefSDK;

namespace Ui {
class LoaderDialog;
}

class LoaderDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit LoaderDialog(QHexEditData* hexeditdata, QWidget *parent = 0);
        bool hasLoaders() const;
        ProcessorLoader *selectedLoader() const;
        ~LoaderDialog();

    private slots:
        void on_loaderTable_doubleClicked(const QModelIndex &);
        void on_loaderTable_clicked(const QModelIndex &index);

    private:
        Ui::LoaderDialog *ui;
        LoaderListModel* _loaderlistmodel;
};

#endif // LOADERDIALOG_H
