#ifndef SEGMENTSDIALOG_H
#define SEGMENTSDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/segmentsmodel/segmentsmodel.h"
#include "prefsdk/disassembler/disassemblerlisting.h"

namespace Ui {
class SegmentsDialog;
}

class SegmentsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit SegmentsDialog(DisassemblerListing* listing, QWidget *parent = 0);
        ~SegmentsDialog();

    private:
        Ui::SegmentsDialog *ui;
};

#endif // SEGMENTSDIALOG_H
