#include "compareview.h"
#include "ui_compareview.h"

CompareView::CompareView(QString leftfile, QString rightfile, QWidget *parent): AbstractView(parent), ui(new Ui::CompareView)
{
    ui->setupUi(this);

    this->_diffcolor = QColor(0xFF, 0xD5, 0xD5);
    this->_samecolor = QColor(0xBF, 0xFF, 0xD0);
    this->_lefthexeditdata = QHexEditData::fromFile(leftfile);
    this->_righthexeditdata = QHexEditData::fromFile(rightfile);

    ui->lblLeft->setText(QString("<b>%1</b>").arg(QDir(leftfile).dirName()));
    ui->lblRight->setText(QString("<b>%1</b>").arg(QDir(rightfile).dirName()));
    ui->hexEditLeft->setData(this->_lefthexeditdata);
    ui->hexEditLeft->setReadOnly(true);
    ui->hexEditRight->setData(this->_righthexeditdata);
    ui->hexEditRight->setReadOnly(true);

    this->createToolbar(ui->hexEditLeft, ui->tbContainerLeft, ui->actionWidgetLeft);
    this->createToolbar(ui->hexEditRight, ui->tbContainerRight, ui->actionWidgetRight);

    connect(ui->actionWidgetLeft, SIGNAL(actionDisplayed()), ui->actionWidgetRight, SLOT(hideAllActions()));
    connect(ui->actionWidgetRight, SIGNAL(actionDisplayed()), ui->actionWidgetLeft, SLOT(hideAllActions()));
    connect(ui->hexEditLeft, SIGNAL(positionChanged(qint64)), this, SLOT(updateLeftInfo(qint64)));
    connect(ui->hexEditLeft, SIGNAL(verticalScrollBarValueChanged(int)), ui->hexEditRight, SLOT(setVerticalScrollBarValue(int)));
    connect(ui->hexEditRight, SIGNAL(positionChanged(qint64)), this, SLOT(updateRightInfo(qint64)));
    connect(ui->hexEditRight, SIGNAL(verticalScrollBarValueChanged(int)), ui->hexEditLeft, SLOT(setVerticalScrollBarValue(int)));

    if(this->_lefthexeditdata->length() >= this->_righthexeditdata->length())
        connect(ui->hexEditLeft, SIGNAL(visibleLinesChanged()), this, SLOT(compare()));
    else
        connect(ui->hexEditRight, SIGNAL(visibleLinesChanged()), this, SLOT(compare()));

    this->updateLeftInfo(0);
    this->updateRightInfo(0);
    this->compare();
}

CompareView::~CompareView()
{
    delete ui;
}

void CompareView::compare()
{
    qint64 start = qMin(ui->hexEditLeft->visibleStartOffset(), ui->hexEditRight->visibleStartOffset()), i = start;
    qint64 end = qMax(ui->hexEditLeft->visibleEndOffset(), ui->hexEditRight->visibleEndOffset());
    QHexEditDataReader leftreader(this->_lefthexeditdata);
    QHexEditDataReader rightreader(this->_righthexeditdata);

    while(i < end)
    {
        if((i >= this->_lefthexeditdata->length()) || (i >= this->_righthexeditdata->length()))
        {
            if(i >= this->_righthexeditdata->length())
                ui->hexEditLeft->highlightBackground(i, end, this->_diffcolor);
            else
                ui->hexEditRight->highlightBackground(i, end, this->_diffcolor);

            break;
        }

        if(!leftreader.at(i) && !rightreader.at(i))
        {
            i++;
            continue;
        }

        if(leftreader.at(i) == rightreader.at(i))
        {
            qint64 s = i;

            while(((i < this->_lefthexeditdata->length()) && (i < this->_righthexeditdata->length())) && (leftreader.at(i) == rightreader.at(i)))
                i++;

            ui->hexEditLeft->highlightBackground(s, i - 1, this->_samecolor);
            ui->hexEditRight->highlightBackground(s, i - 1, this->_samecolor);
            continue;
        }
        else if(leftreader.at(i) != rightreader.at(i))
        {
            qint64 s = i;

            while(((i < this->_lefthexeditdata->length()) && (i < this->_righthexeditdata->length())) && (leftreader.at(i) != rightreader.at(i)))
                i++;

            ui->hexEditLeft->highlightBackground(s, i - 1, this->_diffcolor);
            ui->hexEditRight->highlightBackground(s, i - 1, this->_diffcolor);
            continue;
        }

        i++;
    }
}

void CompareView::updateLeftInfo(qint64 pos)
{
    ui->lblInfoLeft->setText(QString("Offset: %1h").arg(pos, 8, 16, QLatin1Char('0')));
}

void CompareView::updateRightInfo(qint64 pos)
{
    ui->lblInfoRight->setText(QString("Offset %1h").arg(pos, 8, 16, QLatin1Char('0')));
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
