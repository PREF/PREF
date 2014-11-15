#ifndef DISASSEMBLERDIALOG_H
#define DISASSEMBLERDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/disassemblerlist.h"
#include "viewmodels/disassemblerlistmodel/disassemblerlistmodel.h"

using namespace PrefSDK;

namespace Ui {
class DisassemblerDialog;
}

class DisassemblerDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit DisassemblerDialog(QHexEditData* hexeditdata, QWidget *parent = 0);
        DisassemblerDefinition *selectedDisassembler() const;
        bool canLoadDatabase() const;
        bool hasLoaders() const;
        ~DisassemblerDialog();

    private slots:
        void on_disassemblerTable_doubleClicked(const QModelIndex &);
        void on_disassemblerTable_clicked(const QModelIndex &index);

    private:
        Ui::DisassemblerDialog *ui;
        DisassemblerListModel* _disassemblerlistmodel;
};

#endif // DISASSEMBLERDIALOG_H
