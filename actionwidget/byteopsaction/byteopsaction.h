#ifndef BYTEOPSACTION_H
#define BYTEOPSACTION_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "actionwidget/abstractaction.h"

namespace Ui {
class ByteOpsAction;
}

class ByteOpsAction : public AbstractAction
{
    Q_OBJECT

    public:
        enum ByteOperations {And, Or, Xor, Not, Mod};
        explicit ByteOpsAction(QWidget *parent = 0);
        ~ByteOpsAction();

    signals:
        void byteOperationRequested(uchar value, ByteOpsAction::ByteOperations op);

    private slots:
        void on_cbByteOps_currentIndexChanged(int index);
        void on_pbExecute_clicked();

    protected:
        virtual void showEvent(QShowEvent* e);

    private:
        Ui::ByteOpsAction *ui;
};

#endif // BYTEOPSACTION_H
