#ifndef ENTRYPOINTSDIALOG_H
#define ENTRYPOINTSDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/entrypointsmodel/entrypointsmodel.h"
#include "views/disassemblerview/disassemblerlisting.h"

namespace Ui {
class EntryPointsDialog;
}

class EntryPointsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit EntryPointsDialog(DisassemblerListing* listing, QWidget *parent = 0);
        ~EntryPointsDialog();

    private:
        Ui::EntryPointsDialog *ui;
};

#endif // ENTRYPOINTSDIALOG_H
