#include "hexview.h"
#include "ui_hexview.h"

HexView::HexView(QHexEditData* hexeditdata, const QString& viewname, QLabel *labelinfo, QWidget *parent): AbstractView(viewname, labelinfo, parent), ui(new Ui::HexView), _disassemblerdialog(nullptr), _hexeditdata(hexeditdata), _toolbar(nullptr), _entropyenabled(false)
{
    ui->setupUi(this);
    ui->hSplitter->setStretchFactor(0, 1);
    ui->vSplitter->setStretchFactor(0, 1);
    ui->dataTypesWidget->setData(hexeditdata);
    ui->hexEdit->setData(hexeditdata);

    this->_binaryviewdialog = new BinaryViewDialog(hexeditdata, this);
    this->_binaryviewdialog->setWindowTitle(QString("'%1' Binary View").arg(viewname));

    this->_signaturecolor = QColor(0xFF, 0x8C, 0x8C);

    this->createToolBar();
    this->inspectData(hexeditdata);

    connect(ui->hexEdit, SIGNAL(positionChanged(qint64)), ui->dataTypesWidget->model(), SLOT(setOffset(qint64)));
    connect(ui->hexEdit, SIGNAL(positionChanged(qint64)), this, SLOT(updateOffset(qint64)));
    connect(ui->hexEdit, SIGNAL(selectionChanged(qint64)), this, SLOT(updateSelLength(qint64)));
    connect(ui->hexEdit, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onHexEditCustomContextMenuRequested(QPoint)));
    connect(ui->hexEdit, SIGNAL(verticalScrollBarValueChanged(int)), ui->binaryNavigator, SLOT(renderMap(int)));

    connect(this->_binaryviewdialog, SIGNAL(gotoTriggered(qint64)), ui->hexEdit, SLOT(selectPos(qint64)));
}

void HexView::save()
{
    this->_hexeditdata->save();
}

void HexView::save(QString filename)
{
    QFile f(filename);
    this->_hexeditdata->saveTo(&f);
    f.close();
}

HexView::~HexView()
{
    FormatList::removeLoadedFormat(this->_hexeditdata);
    delete ui;
}

bool HexView::canSave() const
{
    return true;
}

void HexView::updateStatusBar()
{
    QString offset = QString("%1").arg(ui->hexEdit->cursorPos(), ui->hexEdit->addressWidth(), 16, QLatin1Char('0')).toUpper();
    QString size = QString("%1").arg(ui->hexEdit->selectionLength(), ui->hexEdit->addressWidth(), 16, QLatin1Char('0')).toUpper();
    this->updateInfoText(QString("<b>Offset:</b> %1h&nbsp;&nbsp;&nbsp;&nbsp;<b>Size:</b> %2h").arg(offset, size));
}

void HexView::createToolBar()
{
    this->_toolbar = new ActionToolBar(ui->hexEdit, ui->tbContainer);

    this->_tbformat = new QToolButton();
    this->_tbformat->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->_tbformat->setIcon(QIcon(":/misc_icons/res/format.png"));
    this->_tbformat->setText("Formats");
    this->_toolbar->addWidget(this->_tbformat);

    this->_actbyteview = this->_toolbar->addAction(QIcon(":/action_icons/res/entropy.png"), "Map View");
    this->_actbyteview->setCheckable(true);
    this->_actbinaryview = this->_toolbar->addAction(QIcon(":/action_icons/res/binview.png"), "Binary View");
    this->_actdisassembler = this->_toolbar->addAction(QIcon(":/action_icons/res/cpu.png"), "Disassembler");
    this->_actdisassembler->setVisible(false);

    this->_toolbar->addSeparator();
    this->_toolbar->createActions(ui->actionWidget, ActionToolBar::AllActions);

    connect(this->_tbformat, SIGNAL(clicked()), ui->formatWidget, SLOT(loadFormat()));
    connect(this->_actbyteview, SIGNAL(triggered()), this, SLOT(onMapViewTriggered()));
    connect(this->_actbinaryview, SIGNAL(triggered()), this, SLOT(onBinaryViewTriggered()));
    connect(this->_actdisassembler, SIGNAL(triggered()), this, SLOT(onDisassemblerTriggered()));

    QVBoxLayout* vl = new QVBoxLayout();
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(this->_toolbar);

    ui->tbContainer->setLayout(vl);
}

void HexView::inspectData(QHexEditData *hexeditdata)
{
    ui->binaryNavigator->setData(ui->hexEdit);
    ui->chartWidget->plot(hexeditdata);
    ui->signaturesWidget->scan(hexeditdata);
    ui->formatWidget->setData(ui->hexEdit);
    ui->stringsWidget->scan(hexeditdata);

    connect(ui->chartWidget, SIGNAL(workStarted()), this, SLOT(onWorkStarted()));
    connect(ui->signaturesWidget, SIGNAL(workStarted()), this, SLOT(onWorkStarted()));
    connect(ui->formatWidget, SIGNAL(workStarted()), this, SLOT(onWorkStarted()));
    connect(ui->stringsWidget, SIGNAL(workStarted()), this, SLOT(onWorkStarted()));

    connect(ui->chartWidget, SIGNAL(workFinished()), this, SLOT(onWorkFinished()));
    connect(ui->signaturesWidget, SIGNAL(workFinished()), this, SLOT(onWorkFinished()));
    connect(ui->formatWidget, SIGNAL(workFinished()), this, SLOT(onWorkFinished()));
    connect(ui->stringsWidget, SIGNAL(workFinished()), this, SLOT(onWorkFinished()));

    connect(ui->stringsWidget, SIGNAL(gotoTriggered(qint64,qint64)), ui->hexEdit, SLOT(setSelectionRange(qint64,qint64)));
    connect(ui->signaturesWidget, SIGNAL(gotoTriggered(qint64,qint64)), ui->hexEdit, SLOT(setSelectionRange(qint64,qint64)));
    connect(ui->formatWidget, SIGNAL(parseStarted()), this, SLOT(disableFormatButton()));
    connect(ui->formatWidget, SIGNAL(parseFinished(FormatList::FormatId,FormatTree*)), this, SLOT(onFormatParseFinished(FormatList::FormatId,FormatTree*)));
}

void HexView::updateOffset(qint64)
{
    this->updateStatusBar();
}

void HexView::updateSelLength(qint64 size)
{
    this->updateStatusBar();

    if(!this->_toolbar)
        return;

    if(!size)
    {
        this->_toolbar->setEditActionsEnabled(false);
        ui->actionWidget->byteOpsAction()->hide();
    }
    else
        this->_toolbar->setEditActionsEnabled(true);
}

void HexView::onMapViewTriggered()
{
    if(this->_actbyteview->isChecked())
        ui->binaryNavigator->displayEntropy();
    else
        ui->binaryNavigator->displayDefault();
}

void HexView::onBinaryViewTriggered()
{
    if(!this->_binaryviewdialog)
        return;

    if(this->_binaryviewdialog->isVisible())
        this->_binaryviewdialog->raise();
    else
        this->_binaryviewdialog->show();
}

void HexView::onDisassemblerTriggered()
{
    if(!this->_disassemblerdialog)
        return;

    if(this->_disassemblerdialog->isVisible())
        this->_disassemblerdialog->raise();
    else
        this->_disassemblerdialog->show();
}

void HexView::onHexEditCustomContextMenuRequested(const QPoint &pos)
{
    QPoint newpos = ui->hexEdit->mapToGlobal(pos);
    this->_toolbar->actionMenu()->popup(newpos);
}

void HexView::onFormatParseFinished(FormatList::FormatId formatid, FormatTree *formattree)
{
    this->_tbformat->setEnabled(true);

    if(!formatid || !formattree)
    {
        this->_actdisassembler->setVisible(false);
        this->_disassemblerdialog = nullptr;

        this->_tbformat->setPopupMode(QToolButton::DelayedPopup);
        this->_tbformat->setMenu(nullptr);

        ui->formatWidget->resetData();
        return;
    }

    FormatList::Format& format = FormatList::formatFromId(formatid);

    if(format.canDisassemble())
    {
        this->_disassemblerdialog = new DisassemblerDialog(this->_hexeditdata, formattree, this);
        this->_disassemblerdialog->setWindowTitle(QString("'%1' Disassembly").arg(this->viewName()));
        this->_actdisassembler->setVisible(true);
    }
    else
    {
        this->_actdisassembler->setVisible(false);
        this->_disassemblerdialog = nullptr;
    }

    if(format.optionsCount() > 0)
    {
        this->_tbformat->setPopupMode(QToolButton::MenuButtonPopup);
        this->_tbformat->setMenu(new OptionMenu(SDKManager::state(), ui->hexEdit, format));
    }
    else
    {
        this->_tbformat->setPopupMode(QToolButton::DelayedPopup);
        this->_tbformat->setMenu(nullptr);
    }
}

void HexView::disableFormatButton()
{
    this->_tbformat->setEnabled(false);
}

void HexView::onWorkStarted()
{
    int idx = ui->tabWidget->indexOf(qobject_cast<QWidget*>(this->sender()));

    if(idx != -1)
        ui->tabWidget->setTabIcon(idx, QIcon(":/misc_icons/res/busy.png"));
}

void HexView::onWorkFinished()
{
    int idx = ui->tabWidget->indexOf(qobject_cast<QWidget*>(this->sender()));

    if(idx != -1)
        ui->tabWidget->setTabIcon(idx, QIcon());
}

