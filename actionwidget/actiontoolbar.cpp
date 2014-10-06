#include "actiontoolbar.h"

ActionToolBar::ActionToolBar(QHexEdit *hexedit, QWidget *parent): QToolBar(parent)
{
    this->_hexedit = hexedit;
    this->_hexeditdata = hexedit->data();
    this->_actions = ActionToolBar::None;
    this->_ctxmenu = nullptr;
    this->_actionwidget = nullptr;
    this->_actundo = nullptr;
    this->_actredo = nullptr;
    this->_actcut = nullptr;
    this->_actcopy = nullptr;
    this->_actpaste = nullptr;
    this->_actselect = nullptr;
    this->_actselectall = nullptr;
    this->_actfind = nullptr;
    this->_actgoto = nullptr;
    this->_actbyteops = nullptr;
    this->_actexport = nullptr;

    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void ActionToolBar::createActions(ActionWidget* actionwidget, ActionToolBar::Actions act)
{
    this->_ctxmenu = nullptr;
    this->_actionwidget = actionwidget;
    this->_actions = act;

    if(this->isEditable() && (act & ActionToolBar::UndoRedo))
    {
        QUndoStack* undostack = this->_hexedit->data()->undoStack();

        this->_actundo = undostack->createUndoAction(this);
        this->_actundo->setIcon(QIcon(":/action_icons/res/undo.png"));
        this->_actundo->setText("Undo");

        this->_actredo = undostack->createRedoAction(this);
        this->_actredo->setIcon(QIcon(":/action_icons/res/redo.png"));
        this->_actredo->setText("Redo");

        this->addAction(this->_actundo);
        this->addAction(this->_actredo);
    }

    if(act & ActionToolBar::Clipboard)
    {
        this->addToolBarSeparatorIfNeeded();

        if(this->isEditable())
        {
            this->_actcut = this->addAction(QIcon(":/action_icons/res/cut.png"), "Cut");
            this->_actcopy = this->addAction(QIcon(":/action_icons/res/copy.png"), "Copy");
            this->_actpaste = this->addAction(QIcon(":/action_icons/res/paste.png"), "Paste");
            this->_actselect = this->addAction(QIcon(":/action_icons/res/select.png"), "Select");
            this->_actselectall = this->addAction(QIcon(":/action_icons/res/selectall.png"), "Select All");

            connect(this->_actcut, SIGNAL(triggered()), this->_hexedit, SLOT(cut()));
            connect(this->_actcopy, SIGNAL(triggered()), this->_hexedit, SLOT(copy()));
            connect(this->_actpaste, SIGNAL(triggered()), this->_hexedit, SLOT(paste()));
            connect(this->_actselect, SIGNAL(triggered()), this, SLOT(showRangeSelector()));
            connect(this->_actselectall, SIGNAL(triggered()), this->_hexedit, SLOT(selectAll()));

            connect(actionwidget->rangeSelectorAction(), SIGNAL(rangeSelected(qint64,qint64)), this->_hexedit, SLOT(setSelection(qint64,qint64)));
        }
        else
        {
            this->_actcopy = this->addAction(QIcon(":/action_icons/res/copy.png"), "Copy");
            this->_actselect = this->addAction(QIcon(":/action_icons/res/select.png"), "Select");
            this->_actselectall = this->addAction(QIcon(":/action_icons/res/selectall.png"), "Select All");

            connect(this->_actcopy, SIGNAL(triggered()), this->_hexedit, SLOT(copy()));
            connect(this->_actselect, SIGNAL(triggered()), this, SLOT(showRangeSelector()));
            connect(this->_actselectall, SIGNAL(triggered()), this->_hexedit, SLOT(selectAll()));
        }
    }

    if(act & ActionToolBar::Seek)
    {
        this->addToolBarSeparatorIfNeeded();
        this->_actfind = this->addAction(QIcon(":/action_icons/res/find.png"), "Find");
        this->_actgoto = this->addAction(QIcon(":/action_icons/res/goto.png"), "Goto");

        connect(this->_actfind, SIGNAL(triggered()), this->_actionwidget, SLOT(showFind()));
        connect(this->_actgoto, SIGNAL(triggered()), this, SLOT(showGoto()));

        connect(actionwidget->findAction(), SIGNAL(findRequest(QByteArray)), this, SLOT(findRequested(QByteArray)));
        connect(actionwidget->gotoAction(), SIGNAL(addressChanged(qint64)), this, SLOT(gotoAddress(qint64)));
    }

    if(act & ActionToolBar::ByteManipulation)
    {
        this->addToolBarSeparatorIfNeeded();

        if(this->isEditable())
        {
            this->_actbyteops = this->addAction(QIcon(":/action_icons/res/byteop.png"), "Byte Ops");
            connect(this->_actbyteops, SIGNAL(triggered()), this->_actionwidget, SLOT(showByteOps()));
        }

        this->_actexport = this->addAction(QIcon(":/action_icons/res/export.png"), "Export");
        connect(this->_actexport, SIGNAL(triggered()), this, SLOT(showExportDialog()));

        connect(actionwidget->byteOpsAction(), SIGNAL(byteOperationRequested(uchar,ByteOpsAction::ByteOperations)), this, SLOT(byteOpRequested(uchar,ByteOpsAction::ByteOperations)));
    }

    if(this->_actions != ActionToolBar::None)
        this->setEditActionsEnabled(false);
}

void ActionToolBar::setEditActionsEnabled(bool b)
{
    if(this->_actcut)
        this->_actcut->setEnabled(b);

    if(this->_actcopy)
        this->_actcopy->setEnabled(b);

    if(this->_actbyteops)
        this->_actbyteops->setEnabled(b);
}

QMenu *ActionToolBar::actionMenu()
{
    if(!this->_ctxmenu)
    {
        this->_ctxmenu = new QMenu();

        if(this->isEditable() && (this->_actions & ActionToolBar::UndoRedo))
        {
            this->_ctxmenu->addAction(this->_actundo);
            this->_ctxmenu->addAction(this->_actredo);
        }

        if(this->_actions & ActionToolBar::Clipboard)
        {
            this->addMenuSeparatorIfNeeded();

            if(this->isEditable())
            {
                this->_ctxmenu->addAction(this->_actcut);
                this->_ctxmenu->addAction(this->_actcopy);
                this->_ctxmenu->addAction(this->_actpaste);
            }
            else
                this->_ctxmenu->addAction(this->_actcopy);

            this->_ctxmenu->addSeparator();
            this->_ctxmenu->addAction(this->_actselectall);
        }

        if(this->_actions & ActionToolBar::Seek)
        {
            this->addMenuSeparatorIfNeeded();
            this->_ctxmenu->addAction(this->_actfind);
            this->_ctxmenu->addAction(this->_actgoto);

            if(this->isEditable())
                this->_ctxmenu->addAction(this->_actbyteops);
        }
    }

    return this->_ctxmenu;
}

void ActionToolBar::doAnd(qint64 start, qint64 end, uchar value)
{
    if(end > this->_hexeditdata->length())
        end = this->_hexeditdata->length();

    qint64 l = end - start;
    QHexEditDataReader reader(this->_hexeditdata);
    QHexEditDataWriter writer(this->_hexeditdata);
    QByteArray ba = reader.read(start, end - start);

    for(int i = 0; i < ba.length(); i++)
        ba[i] = ba[i] & value;

    writer.replace(start, l, ba);
}

void ActionToolBar::doOr(qint64 end, qint64 start, uchar value)
{
    if(end > this->_hexeditdata->length())
        end = this->_hexeditdata->length();

    qint64 l = end - start;
    QHexEditDataReader reader(this->_hexeditdata);
    QHexEditDataWriter writer(this->_hexeditdata);
    QByteArray ba = reader.read(start, end - start);

    for(int i = 0; i < ba.length(); i++)
        ba[i] = ba[i] | value;

    writer.replace(start, l, ba);
}

void ActionToolBar::doXor(qint64 start, qint64 end, uchar value)
{
    if(end > this->_hexeditdata->length())
        end = this->_hexeditdata->length();

    qint64 l = end - start;
    QHexEditDataReader reader(this->_hexeditdata);
    QHexEditDataWriter writer(this->_hexeditdata);
    QByteArray ba = reader.read(start, end - start);

    for(int i = 0; i < ba.length(); i++)
        ba[i] = ba[i] ^ value;

    writer.replace(start, l, ba);
}

void ActionToolBar::doMod(qint64 start, qint64 end, uchar value)
{
    if(end > this->_hexeditdata->length())
        end = this->_hexeditdata->length();

    qint64 l = end - start;
    QHexEditDataReader reader(this->_hexeditdata);
    QHexEditDataWriter writer(this->_hexeditdata);
    QByteArray ba = reader.read(start, end - start);

    for(int i = 0; i < ba.length(); i++)
        ba[i] = ba[i] % value;

    writer.replace(start, l, ba);
}

void ActionToolBar::doNot(qint64 start, qint64 end)
{
    if(end > this->_hexeditdata->length())
        end = this->_hexeditdata->length();

    qint64 l = end - start;
    QHexEditDataReader reader(this->_hexeditdata);
    QHexEditDataWriter writer(this->_hexeditdata);
    QByteArray ba = reader.read(start, end - start);

    for(int i = 0; i < ba.length(); i++)
        ba[i] = !ba[i];

    writer.replace(start, l, ba);
}

void ActionToolBar::addMenuSeparatorIfNeeded()
{
    if(!this->_ctxmenu->actions().isEmpty())
        this->_ctxmenu->addSeparator();
}

void ActionToolBar::addToolBarSeparatorIfNeeded()
{
    if(!this->actions().isEmpty())
        this->addSeparator();
}

bool ActionToolBar::isEditable()
{
    return (this->_actions & ActionToolBar::NoEditable) == 0;
}

void ActionToolBar::showRangeSelector()
{
    this->_actionwidget->showRangeSelector(this->_hexedit->data()->length());
}

void ActionToolBar::showGoto()
{
    this->_actionwidget->showGoto(this->_hexedit->data()->length());
}

void ActionToolBar::showExportDialog()
{
    ExportDialog ed(this->_hexedit, this->topLevelWidget());
    int res = ed.exec();

    if(res == ExportDialog::Accepted)
        ed.selectedExporter()->callDump(this->_hexeditdata, ed.fileName(), ed.startOffset(), ed.endOffset());
}

void ActionToolBar::byteOpRequested(uchar value, ByteOpsAction::ByteOperations bo)
{
    switch(bo)
    {
        case ByteOpsAction::And:
            this->doAnd(this->_hexedit->selectionStart(), this->_hexedit->selectionEnd(), value);
            break;

        case ByteOpsAction::Or:
            this->doOr(this->_hexedit->selectionStart(), this->_hexedit->selectionEnd(), value);
            break;

        case ByteOpsAction::Xor:
            this->doXor(this->_hexedit->selectionStart(), this->_hexedit->selectionEnd(), value);
            break;

        case ByteOpsAction::Mod:
            this->doMod(this->_hexedit->selectionStart(), this->_hexedit->selectionEnd(), value);
            break;

        case ByteOpsAction::Not:
            this->doNot(this->_hexedit->selectionStart(), this->_hexedit->selectionEnd());
            break;

        default:
            break;
    }
}

void ActionToolBar::findRequested(QByteArray value)
{
    qint64 idx = this->_hexedit->indexOf(value, this->_hexedit->selectionEnd());

    if(idx != -1)
        this->_hexedit->setSelection(idx, idx + value.length());
    else
    {
        this->_hexedit->setCursorPos(this->_hexedit->cursorPos()); /* Reset Selection */

        QMessageBox m(this);
        m.setWindowTitle("Search Result");
        m.setText("Text not Found");
        m.exec();
    }
}

void ActionToolBar::gotoAddress(qint64 address)
{
    this->_hexedit->setSelection(address, address + 1);
}
