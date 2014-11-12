#include "renamewidget.h"
#include "ui_renamewidget.h"

RenameWidget::RenameWidget(QWidget *parent): QWidget(parent), ui(new Ui::RenameWidget), _block(nullptr)
{
    ui->setupUi(this);
    this->setVisible(false);

    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->pbRename, SIGNAL(clicked()), this, SLOT(doRenameRequest()));
    connect(ui->leName, SIGNAL(returnPressed()), this, SLOT(doRenameRequest()));
}

void RenameWidget::setLabel(const QString& name)
{
    ui->lblRename->setText(QString("Rename %1:").arg(name));
}

void RenameWidget::setBlock(Block *block)
{
    this->_block = block;
}

RenameWidget::~RenameWidget()
{
    delete ui;
}

void RenameWidget::doRenameRequest()
{
    if(this->_block)
        emit renameRequested(this->_block, ui->leName->text());

    this->hide();
}

void RenameWidget::hideEvent(QHideEvent *e)
{
    ui->leName->clear();
    QWidget::hideEvent(e);
}

void RenameWidget::showEvent(QShowEvent *e)
{
    ui->leName->setFocus();
    QWidget::showEvent(e);
}

void RenameWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
        this->hide();

    QWidget::keyPressEvent(e);
}
