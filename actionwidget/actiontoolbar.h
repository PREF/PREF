#ifndef ACTIONTOOLBAR_H
#define ACTIONTOOLBAR_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexedit.h"
#include "prefsdk/io/bytebuffer.h"
#include "actionwidget.h"
#include "exportdialog.h"

using namespace PrefSDK;

class ActionToolBar : public QToolBar
{
    Q_OBJECT

    public:
        enum Actions {None = 0, UndoRedo = 1, Clipboard = 2, Seek = 4, ByteManipulation = 8, AllActions = UndoRedo | Clipboard | Seek | ByteManipulation, NoEditable = 0x1000 };
        explicit ActionToolBar(QHexEdit* hexedit, ByteBuffer* bytebuffer, QWidget *parent = 0);
        void createActions(ActionWidget* actionwidget, ActionToolBar::Actions act);
        void setEditActionsEnabled(bool b);
        QMenu* actionMenu();

    private:
        void doAnd(qint64 start, qint64 end, uchar value);
        void doOr(qint64 end, qint64 start, uchar value);
        void doXor(qint64 start, qint64 end, uchar value);
        void doMod(qint64 start, qint64 end, uchar value);
        void doNot(qint64 start, qint64 end);
        void addMenuSeparatorIfNeeded();
        void addToolBarSeparatorIfNeeded();
        bool isEditable();

    private slots:
        void showRangeSelector();
        void showGoto();
        void showExportDialog();
        void byteOpRequested(uchar value, ByteOpsAction::ByteOperations bo);
        void findRequested(QByteArray value);
        void gotoAddress(qint64 address);

    private:
        QMenu* _ctxmenu;
        QHexEdit* _hexedit;
        ByteBuffer* _bytebuffer;
        ActionWidget* _actionwidget;
        ActionToolBar::Actions _actions;
        QAction* _actundo;
        QAction* _actredo;
        QAction* _actcut;
        QAction* _actcopy;
        QAction* _actpaste;
        QAction* _actselect;
        QAction* _actselectall;
        QAction* _actfind;
        QAction* _actgoto;
        QAction* _actbyteops;
        QAction* _actexport;
};

#endif // ACTIONTOOLBAR_H
