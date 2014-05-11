#ifndef BINARYVIEWDIALOG_H
#define BINARYVIEWDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"

namespace Ui {
class BinaryViewDialog;
}

class BinaryViewDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit BinaryViewDialog(QHexEditData *hexeditdata, QWidget *parent = 0);
        ~BinaryViewDialog();

    private slots:
        void onCtrlModifier(bool pressed);
        void onShiftModifier(bool pressed);

    protected:
        virtual void closeEvent(QCloseEvent* event);
        virtual void keyPressEvent(QKeyEvent* event);
        virtual void keyReleaseEvent(QKeyEvent* event);

    signals:
        void gotoTriggered(qint64);

    private:
        Ui::BinaryViewDialog *ui;
        QHexEditData* _hexeditdata;
        bool _ctrlpressed;
        bool _shiftpressed;
};

#endif // BINARYVIEWDIALOG_H
