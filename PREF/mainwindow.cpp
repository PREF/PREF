#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/views/binaryview.h"
#include <QDesktopWidget>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), _currentview(NULL)
{
    ui->setupUi(this);
    this->centerWindow();
    this->updateToolBar(NULL);

    this->_lblstatus = new QLabel();
    ui->statusBar->addPermanentWidget(this->_lblstatus, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::centerWindow()
{
    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
}

void MainWindow::updateToolBar(AbstractView *abstractview) const
{
    ui->toolBar->clear();
    ui->toolBar->setToolButtonStyle(abstractview ? Qt::ToolButtonTextUnderIcon : Qt::ToolButtonTextBesideIcon);
    ui->toolBar->addAction(ui->action_Analyze);

    if(abstractview)
        abstractview->updateToolBar(ui->toolBar);

    ui->toolBar->addAction(ui->action_About_PREF);
}

void MainWindow::on_action_Analyze_triggered()
{
    QString file = QFileDialog::getOpenFileName(this);

    if(file.isEmpty())
        return;

    this->_currentview = new BinaryView(QHexDocument::fromFile(file), this->_lblstatus, file, this);
    this->setCentralWidget(this->_currentview);
    this->updateToolBar(this->_currentview);
}
