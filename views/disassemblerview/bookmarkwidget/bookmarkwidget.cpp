#include "bookmarkwidget.h"
#include "ui_bookmarkwidget.h"

BookmarkWidget::BookmarkWidget(QWidget *parent): QWidget(parent), ui(new Ui::BookmarkWidget), _block(nullptr)
{
    ui->setupUi(this);
    this->setVisible(false);

    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->pbSave, SIGNAL(clicked()), this, SLOT(doSaveRequest()));
    connect(ui->leDescription, SIGNAL(returnPressed()), this, SLOT(doSaveRequest()));
}

void BookmarkWidget::setBlock(Block *block)
{
    this->_block = block;
    //FIXME: ui->lblBookmark->setText(QString("Bookmark at %1:").arg(block->startAddress().toString(16)));
}

BookmarkWidget::~BookmarkWidget()
{
    delete ui;
}

void BookmarkWidget::doSaveRequest()
{
    if(this->_block)
        emit saveBookmarkRequested(this->_block, ui->leDescription->text());

    this->hide();
}

void BookmarkWidget::hideEvent(QHideEvent* e)
{
    ui->leDescription->clear();
    QWidget::hideEvent(e);
}

void BookmarkWidget::showEvent(QShowEvent* e)
{
    ui->leDescription->setFocus();
    QWidget::showEvent(e);
}

void BookmarkWidget::keyPressEvent(QKeyEvent* e)
{
    if(e->key() == Qt::Key_Escape)
        this->hide();

    QWidget::keyPressEvent(e);
}
