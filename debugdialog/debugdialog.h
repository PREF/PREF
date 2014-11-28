#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include "stackdumphighlighter.h"
#include "lua.hpp"

namespace Ui {
class DebugDialog;
}

class DebugDialog : public QDialog
{
    Q_OBJECT

    private:
        explicit DebugDialog(lua_State *l, QWidget *parent = 0);

    public:
        static void createInstance(lua_State* l);
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
        lua_State* _state;
        StackDumpHighlighter* _stackdumphighlighter;
        static DebugDialog* _instance;
};

#endif // DEBUGDIALOG_H
