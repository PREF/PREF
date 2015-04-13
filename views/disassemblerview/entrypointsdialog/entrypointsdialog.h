#ifndef ENTRYPOINTSDIALOG_H
#define ENTRYPOINTSDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/entrypointsmodel/entrypointsmodel.h"

namespace Ui {
class EntryPointsDialog;
}

class EntryPointsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit EntryPointsDialog(DisassemblerListing* listing, QWidget *parent = 0);
        Function* selectedFunction() const;
        ~EntryPointsDialog();

    private slots:
        void on_entryPointsTable_doubleClicked(const QModelIndex &index);
        void on_buttonBox_accepted();

    private:
        Ui::EntryPointsDialog *ui;
        Function* _selectedfunction;
};

#endif // ENTRYPOINTSDIALOG_H
