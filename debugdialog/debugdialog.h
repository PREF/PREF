#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include "stackdumphighlighter.h"
#include <prefcontext.h>

namespace Ui {
class DebugDialog;
}

using namespace PrefLib;

class DebugDialog : public QDialog
{
    Q_OBJECT

    private:
        explicit DebugDialog(QWidget *parent = 0);

    public:
        static void createInstance();
        static DebugDialog* instance();
        ~DebugDialog();

    public slots:
        DebugDialog* out(QString s);

    private:
        QString stackdump();
        QString traceback();
        QString typeValue(int idx);
        int tableLength(int idx);

    private slots:
        void on_pbClose_clicked();
        void on_pbTerminate_clicked();

    protected:
        virtual void closeEvent(QCloseEvent* e);
        virtual void showEvent(QShowEvent*);

    private:
        Ui::DebugDialog *ui;
        StackDumpHighlighter* _stackdumphighlighter;
        static DebugDialog* _instance;
};

#endif // DEBUGDIALOG_H
