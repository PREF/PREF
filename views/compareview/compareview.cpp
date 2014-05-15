#include "compareview.h"
#include "ui_compareview.h"

CompareView::CompareView(QString leftfile, QString rightfile, const QString& viewname, QLabel *labelinfo, QWidget *parent): AbstractView(viewname, labelinfo, parent), ui(new Ui::CompareView)
{
    ui->setupUi(this);

    QList<int> l;
    l.append(this->width() / 2);
    l.append(this->width() / 2);

    ui->hSplitter->setSizes(l);
    ui->vSplitter->setStretchFactor(0, 1);

    this->_diffcolor = QColor(0xFF, 0xD5, 0xD5);
    this->_lefthexeditdata = QHexEditData::fromFile(leftfile);
    this->_righthexeditdata = QHexEditData::fromFile(rightfile);

    ui->lblLeft->setText(QString("<b>%1</b>").arg(QDir(leftfile).dirName()));
    ui->lblRight->setText(QString("<b>%1</b>").arg(QDir(rightfile).dirName()));
    ui->hexEditLeft->setData(this->_lefthexeditdata);
    ui->hexEditLeft->setReadOnly(true);
    ui->hexEditRight->setData(this->_righthexeditdata);
    ui->hexEditRight->setReadOnly(true);

    this->_comparemodel = new CompareModel(ui->diffList);
    ui->diffList->setModel(this->_comparemodel);

    this->_worker.setData(this->_lefthexeditdata, this->_righthexeditdata);

    this->createToolbar(ui->hexEditLeft, ui->tbContainerLeft, ui->actionWidgetLeft);
    this->createToolbar(ui->hexEditRight, ui->tbContainerRight, ui->actionWidgetRight);

    connect(ui->actionWidgetLeft, SIGNAL(actionDisplayed()), ui->actionWidgetRight, SLOT(hideAllActions()));
    connect(ui->actionWidgetRight, SIGNAL(actionDisplayed()), ui->actionWidgetLeft, SLOT(hideAllActions()));
    connect(ui->hexEditLeft, SIGNAL(positionChanged(qint64)), this, SLOT(updateLeftInfo(qint64)));
    connect(ui->hexEditLeft, SIGNAL(verticalScrollBarValueChanged(int)), ui->hexEditRight, SLOT(setVerticalScrollBarValue(int)));
    connect(ui->hexEditRight, SIGNAL(positionChanged(qint64)), this, SLOT(updateRightInfo(qint64)));
    connect(ui->hexEditRight, SIGNAL(verticalScrollBarValueChanged(int)), ui->hexEditLeft, SLOT(setVerticalScrollBarValue(int)));
    connect(&this->_worker, SIGNAL(finished()), this, SLOT(onCompareWorkerFinished()));

    if(this->_lefthexeditdata->length() >= this->_righthexeditdata->length())
        connect(ui->hexEditLeft, SIGNAL(visibleLinesChanged()), this, SLOT(highlightDifferences()));
    else
        connect(ui->hexEditRight, SIGNAL(visibleLinesChanged()), this, SLOT(highlightDifferences()));

    this->_worker.start(QThread::LowPriority);
}

CompareView::~CompareView()
{
    delete ui;
}

void CompareView::onCompareWorkerFinished()
{
    this->_offsetlist = this->_worker.offsetList();
    this->_differencemap = this->_worker.differences();

    this->_comparemodel->setData(this->_worker.offsetList(), this->_worker.differences());
    this->highlightDifferences();
}

void CompareView::updateLeftInfo(qint64)
{
    this->updateStatusBar();
}

void CompareView::updateRightInfo(qint64)
{
    this->updateStatusBar();
}

void CompareView::highlightDifferences()
{
    if(this->_offsetlist.isEmpty())
        return;

    QHexEdit* hexedit = ((this->_lefthexeditdata->length() >= this->_righthexeditdata->length()) ? ui->hexEditLeft : ui->hexEditRight);
    qint64 offset = hexedit->visibleStartOffset(), endoffset = hexedit->visibleEndOffset();

    while(offset <= endoffset)
    {
        if(!this->_differencemap.contains(offset))
        {
            offset++;
            continue;
        }

        qint64 diffend = this->_differencemap[offset];
        ui->hexEditLeft->highlightBackground(offset, diffend, this->_diffcolor);
        ui->hexEditRight->highlightBackground(offset, diffend, this->_diffcolor);
        offset = diffend + 1;
    }
}

void CompareView::createToolbar(QHexEdit* hexedit, QWidget *tbcontainer, ActionWidget* actionwidget)
{
    ActionToolBar* atb = new ActionToolBar(hexedit);
    atb->createActions(actionwidget, static_cast<ActionToolBar::Actions>(ActionToolBar::Clipboard | ActionToolBar::Seek | ActionToolBar::ByteManipulation | ActionToolBar::NoEditable));

    QVBoxLayout* vl = new QVBoxLayout();
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(atb);

    tbcontainer->setLayout(vl);
}

bool CompareView::canSave() const
{
    return false;
}

void CompareView::updateStatusBar()
{
    QString leftoffset = QString("%1").arg(ui->hexEditLeft->cursorPos(), ui->hexEditLeft->addressWidth(), 16, QLatin1Char('0')).toUpper();
    QString rightoffset = QString("%1").arg(ui->hexEditRight->cursorPos(), ui->hexEditRight->addressWidth(), 16, QLatin1Char('0')).toUpper();
    this->updateInfoText(QString("<b>Left Offset</b>: %1h&nbsp;&nbsp;&nbsp;&nbsp;<b>Right Offset</b>: %2h").arg(leftoffset, rightoffset));
}
