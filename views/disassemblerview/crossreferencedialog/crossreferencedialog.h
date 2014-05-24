#ifndef CROSSREFERENCEDIALOG_H
#define CROSSREFERENCEDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/crossreferencemodel/crossreferencemodel.h"

namespace Ui {
class CrossReferenceDialog;
}

class CrossReferenceDialog : public QDialog
{
    Q_OBJECT
    
    public:
        explicit CrossReferenceDialog(QWidget *parent = 0);
        ~CrossReferenceDialog();
    
    private slots:
        void on_tvXRefs_doubleClicked(const QModelIndex &index);
        void on_buttonBox_accepted();

    private:
        Ui::CrossReferenceDialog *ui;
        CrossReferenceModel* _xrefmodel;
};

#endif // CROSSREFERENCEDIALOG_H
