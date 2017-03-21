#include "compareview.h"
#include "ui_compareview.h"

CompareView::CompareView(QString leftfile, QString rightfile, QLabel *labelinfo, QWidget *parent): AbstractView(nullptr, QString(), labelinfo, parent), ui(new Ui::CompareView)
{
    ui->setupUi(this);

    QList<int> l;
    l.append(this->width() / 2);
    l.append(this->width() / 2);

    ui->hSplitter->setSizes(l);
    ui->vSplitter->setStretchFactor(0, 1);

    this->_diffcolor = QColor(0xFF, 0xD5, 0xD5);
    this->_samecolor = QColor(0xBF, 0xFF, 0xD0);
    this->_lefthexeditdata = QHexEditData::fromFile(leftfile);
    this->_righthexeditdata = QHexEditData::fromFile(rightfile);
    this->_leftreader = new QHexEditDataReader(this->_lefthexeditdata, this);
    this->_rightreader = new QHexEditDataReader(this->_righthexeditdata, this);

    ui->lblLeft->setText(QString("<b>%1</b>").arg(QDir(leftfile).dirName()));
    ui->lblRight->setText(QString("<b>%1</b>").arg(QDir(rightfile).dirName()));
    ui->hexEditLeft->setData(this->_lefthexeditdata);
    ui->hexEditLeft->setReadOnly(true);
    ui->hexEditRight->setData(this->_righthexeditdata);
    ui->hexEditRight->setReadOnly(true);

    this->_comparemodel = nullptr;
    this->_worker = new CompareWorker(ui->hexEditLeft, ui->hexEditRight);

    this->createToolbar(ui->hexEditLeft, ui->tbContainerLeft, ui->actionWidgetLeft);
    this->createToolbar(ui->hexEditRight, ui->tbContainerRight, ui->actionWidgetRight);

    connect(ui->actionWidgetLeft, SIGNAL(actionDisplayed()), ui->actionWidgetRight, SLOT(hideAllActions()));
    connect(ui->actionWidgetRight, SIGNAL(actionDisplayed()), ui->actionWidgetLeft, SLOT(hideAllActions()));
    connect(ui->hexEditLeft, SIGNAL(positionChanged(qint64)), this, SLOT(updateLeftInfo(qint64)));
    connect(ui->hexEditLeft, SIGNAL(verticalScrollBarValueChanged(int)), ui->hexEditRight, SLOT(setVerticalScrollBarValue(int)));
    connect(ui->hexEditRight, SIGNAL(positionChanged(qint64)), this, SLOT(updateRightInfo(qint64)));
    connect(ui->hexEditRight, SIGNAL(verticalScrollBarValueChanged(int)), ui->hexEditLeft, SLOT(setVerticalScrollBarValue(int)));
    connect(this->_worker, SIGNAL(finished()), this, SLOT(onCompareWorkerFinished()));

    if(this->_lefthexeditdata->length() >= this->_righthexeditdata->length())
        connect(ui->hexEditLeft, SIGNAL(visibleLinesChanged()), this, SLOT(highlightDifferences()));
    else
        connect(ui->hexEditRight, SIGNAL(visibleLinesChanged()), this, SLOT(highlightDifferences()));

    this->highlightDifferences();
    this->_worker->start();
}

CompareView::~CompareView()
{
    delete ui;
}

void CompareView::onCompareWorkerFinished()
{
    this->_offsetlist = this->_worker->offsetList();
    this->_differencemap = this->_worker->differences();

    this->_comparemodel = new CompareModel(this->_offsetlist, this->_differencemap, this);
    ui->diffList->setModel(this->_comparemodel);
    this->updateStatusBar();
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
    QHexEdit* hexedit = ((this->_lefthexeditdata->length() >= this->_righthexeditdata->length()) ? ui->hexEditLeft : ui->hexEditRight);
    qint64 startoffset = hexedit->visibleStartOffset(), endoffset = hexedit->visibleEndOffset(), offset = startoffset;

    if(startoffset < this->_lefthexeditdata->length())
        ui->hexEditLeft->clearHighlight();

    if(startoffset < this->_righthexeditdata->length())
        ui->hexEditRight->clearHighlight();

    while(offset <= endoffset)
    {
        if(offset >= this->_lefthexeditdata->length())
        {
            ui->hexEditRight->highlightBackground(offset, endoffset, this->_diffcolor);
            break;
        }
        else if(offset >= this->_righthexeditdata->length())
        {
            ui->hexEditLeft->highlightBackground(offset, endoffset, this->_diffcolor);
            break;
        }

        uchar leftch = this->_leftreader->at(offset), rightch = this->_rightreader->at(offset);

        if(!leftch && !rightch)
        {
            offset++;
            continue;
        }

        if(leftch == rightch)
        {
            this->highlightSameBytes(offset);
            continue;
        }
        else if(leftch != rightch)
        {
            this->highlightDiffBytes(offset);
            continue;
        }
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

void CompareView::highlightDiffBytes(qint64 &offset)
{
    qint64 diffstartoffset = offset++;

    while(((offset < this->_lefthexeditdata->length()) && (offset < this->_righthexeditdata->length())) && (this->_leftreader->at(offset) != this->_rightreader->at(offset)))
        offset++;

    ui->hexEditRight->highlightBackground(diffstartoffset, offset - 1, this->_diffcolor);
    ui->hexEditLeft->highlightBackground(diffstartoffset, offset - 1, this->_diffcolor);
}

void CompareView::highlightSameBytes(qint64 &offset)
{
    qint64 diffstartoffset = offset++;

    while(((offset < this->_lefthexeditdata->length()) && (offset < this->_righthexeditdata->length())) && (this->_leftreader->at(offset) == this->_rightreader->at(offset)))
        offset++;

    ui->hexEditRight->highlightBackground(diffstartoffset, offset - 1, this->_samecolor);
    ui->hexEditLeft->highlightBackground(diffstartoffset, offset - 1, this->_samecolor);
}

bool CompareView::canSave() const
{
    return false;
}

void CompareView::updateStatusBar()
{
    QString status;
    QString leftoffset = QString("%1").arg(ui->hexEditLeft->cursorPos(), ui->hexEditLeft->addressWidth(), 16, QLatin1Char('0')).toUpper();
    QString rightoffset = QString("%1").arg(ui->hexEditRight->cursorPos(), ui->hexEditRight->addressWidth(), 16, QLatin1Char('0')).toUpper();

    if(this->_worker->isRunning())
        status = "Comparing...";

    this->updateInfoText(QString("<b>Left Offset</b>: %1h&nbsp;&nbsp;&nbsp;&nbsp;<b>Right Offset</b>: %2h\t %3").arg(leftoffset, rightoffset, status));
}

LogWidget *CompareView::logWidget()
{
    return nullptr;
}

void CompareView::on_diffList_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    qint64 startoffset = this->_offsetlist[index.row()];
    qint64 endoffset = this->_differencemap[startoffset];

    ui->hexEditLeft->setSelection(startoffset, endoffset);
    ui->hexEditRight->setSelection(startoffset, endoffset);
}
