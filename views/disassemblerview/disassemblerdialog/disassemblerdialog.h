#ifndef DISASSEMBLERDIALOG_H
#define DISASSEMBLERDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/disassembler/disassembler.h"
#include "viewmodels/loaderlistmodel/loaderlistmodel.h"

using namespace PrefSDK;

namespace Ui {
class DisassemblerDialog;
}

class DisassemblerDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit DisassemblerDialog(QHexEditData* hexeditdata, QWidget *parent = 0);
        LoaderList::LoaderId selectedLoader() const;
        ~DisassemblerDialog();

    private:
        Ui::DisassemblerDialog *ui;
        LoaderListModel* _loaderlistmodel;
};

#endif // DISASSEMBLERDIALOG_H
