#include "hexview.h"
#include "ui_hexview.h"

HexView::HexView(QHexEditData* hexeditdata, const QString& viewname, QLabel *labelinfo, QWidget *parent): AbstractView(hexeditdata, viewname, labelinfo, parent), ui(new Ui::HexView), _toolbar(nullptr), _entropyenabled(false)
{
    ui->setupUi(this);
    ui->hSplitter->setStretchFactor(0, 1);
    ui->vSplitter->setStretchFactor(0, 1);
    ui->hexEdit->setData(hexeditdata);
    ui->formatWidget->setLogWidget(ui->tabOutput->logWidget());
    ui->tabView->tabBar()->hide();

    this->_signaturecolor = QColor(0xFF, 0x8C, 0x8C);

    this->createToolBar();
    this->inspectData(hexeditdata);

    connect(ui->hexEdit, SIGNAL(positionChanged(qint64)), ui->dataTypesWidget->model(), SLOT(setOffset(qint64)));
    connect(ui->hexEdit, SIGNAL(positionChanged(qint64)), this, SLOT(updateOffset(qint64)));
    connect(ui->hexEdit, SIGNAL(selectionChanged(qint64)), this, SLOT(updateSelLength(qint64)));
    connect(ui->hexEdit, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onHexEditCustomContextMenuRequested(QPoint)));
    connect(ui->hexEdit, SIGNAL(verticalScrollBarValueChanged(int)), ui->binaryNavigator, SLOT(renderMap(int)));
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
    delete ui;
}

QHexEditData *HexView::data()
{
    return this->_hexeditdata;
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

    this->_toolbar->addSeparator();
    this->_toolbar->createActions(ui->actionWidget, ActionToolBar::AllActions);

    connect(this->_tbformat, SIGNAL(clicked()), ui->formatWidget, SLOT(loadFormat()));
    connect(this->_actbyteview, SIGNAL(triggered()), this, SLOT(onMapViewTriggered()));

    QVBoxLayout* vl = new QVBoxLayout();
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(this->_toolbar);

    ui->tbContainer->setLayout(vl);
}

void HexView::hideFormatView()
{
    ui->tabView->tabBar()->hide();
    ui->tabView->setCurrentIndex(0);

    if(ui->tabView->count() > 1)
        ui->tabView->removeTab(1);
}

void HexView::inspectData(QHexEditData *hexeditdata)
{
    ui->dataTypesWidget->setData(hexeditdata);
    ui->visualMapWidget->map(ui->hexEdit);
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

    connect(ui->visualMapWidget, SIGNAL(gotoTriggered(qint64)), ui->hexEdit, SLOT(selectPos(qint64)));
    connect(ui->stringsWidget, SIGNAL(gotoTriggered(qint64,qint64)), ui->hexEdit, SLOT(setSelectionRange(qint64,qint64)));
    connect(ui->signaturesWidget, SIGNAL(gotoTriggered(qint64,qint64)), ui->hexEdit, SLOT(setSelectionRange(qint64,qint64)));
    connect(ui->formatWidget, SIGNAL(parsingStarted()), this, SLOT(onFormatParsingStarted()));
    connect(ui->formatWidget, SIGNAL(parsingCompleted()), this, SLOT(onFormatParsingFinished()));
    connect(ui->formatWidget, SIGNAL(parsingFailed()), this, SLOT(onFormatParsingFailed()));
}

void HexView::selectPage(QWidget *page)
{
    int idx = ui->tabWidget->indexOf(page);

    if(idx != -1)
        ui->tabWidget->setCurrentIndex(idx);
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

void HexView::onHexEditCustomContextMenuRequested(const QPoint &pos)
{
    QPoint newpos = ui->hexEdit->mapToGlobal(pos);
    this->_toolbar->actionMenu()->popup(newpos);
}

void HexView::onFormatParsingStarted()
{
    this->_tbformat->setEnabled(false);
}

void HexView::onFormatParsingFinished()
{
    this->_tbformat->setEnabled(true);

    if(ui->formatWidget->formatView())
    {
        ui->tabView->addTab(ui->formatWidget->formatView(), "Format View");
        ui->tabView->tabBar()->setVisible(true);
    }
    else
        this->hideFormatView();
}

void HexView::onFormatParsingFailed()
{
    this->_tbformat->setEnabled(true);
    ui->formatWidget->resetData();

    this->hideFormatView();
    this->selectPage(ui->tabOutput);
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

