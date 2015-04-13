#ifndef SEGMENTSDIALOG_H
#define SEGMENTSDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/segmentsmodel/segmentsmodel.h"

namespace Ui {
class SegmentsDialog;
}

class SegmentsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit SegmentsDialog(DisassemblerListing* listing, QWidget *parent = 0);
        Segment* selectedSegment() const;
        ~SegmentsDialog();

    private slots:
        void on_segmentTable_doubleClicked(const QModelIndex &index);
        void on_buttonBox_accepted();

    private:
        Ui::SegmentsDialog *ui;
        Segment* _selectedsegment;
};

#endif // SEGMENTSDIALOG_H
