#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include "viewmodels/luatypeviewmodel/luastackviewmodel.h"

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
        DebugDialog* luaOut(QString s);
        DebugDialog* out(QString s);
        DebugDialog* outWord(QString s);
        DebugDialog* out(lua_Integer i, int base = 10, int fieldwidth = 0);
        DebugDialog* hexDump(QByteArray ba);
        DebugDialog* newLine(int count = 1);

    signals:
        void outHtml(QString);
        void outText(QString);
        void luaOutText(QString);

    private:
        QString stackDump();
        QString traceback();

    private slots:
        void on_pbClose_clicked();
        void on_pbTerminate_clicked();

    protected:
        virtual void closeEvent(QCloseEvent* e);
        virtual void showEvent(QShowEvent*);

    private:
        Ui::DebugDialog *ui;
        lua_State* _state;
        LuaStackViewModel* _stackmodel;
        static DebugDialog* _instance;
};

#endif // DEBUGDIALOG_H
