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
        bool elaborateInstructions() const;
        bool analyzeListing() const;
        bool hasLoaders() const;
        DisassemblerDefinition *selectedDisassembler() const;
        ~DisassemblerDialog();

    private slots:
        void on_disassemblerTable_doubleClicked(const QModelIndex &);
        void on_disassemblerTable_clicked(const QModelIndex &index);
        void on_cbElaborateInstructions_stateChanged(int state);

    private:
        Ui::DisassemblerDialog *ui;
        DisassemblerListModel* _disassemblerlistmodel;
};

#endif // DISASSEMBLERDIALOG_H
