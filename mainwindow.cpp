#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->actions()[2]->setVisible(false); /* Hide First Separator */
    this->setAcceptDrops(true);
    this->centerWindowToScreen();

    this->_lblinfo = new QLabel();
    this->_basetitle = this->windowTitle();
    ui->statusBar->addWidget(this->_lblinfo, 1);

    DebugDialog::createInstance();
    this->_sdkloaded = SDKManager::loadSDK();

    if(!this->_sdkloaded)
    {
        QMessageBox m;
        m.setWindowTitle("Lua Initialization Failed");
        m.setText("Cannot initialize Lua Interpreter (SDK is missing?)");
        m.exec();
    }
    else
        this->checkCommandLine();
}

bool MainWindow::sdkLoaded()
{
    return this->_sdkloaded;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *e)
{
    if(e->mimeData()->hasUrls())
         e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
    bool accept = false;
    const QMimeData* mimedata = e->mimeData();

    if(mimedata->hasUrls())
    {
        QList<QUrl> urllist = mimedata->urls();

        for(int i = 0; (i < urllist.length()) && (i < 10); i++)
        {
            QString locfile = urllist[i].toLocalFile();
            QFileInfo fi(locfile);

            if(fi.isFile())
            {
                accept = true;
                this->loadFile(urllist[i].toLocalFile());
            }
        }

        if(accept)
            e->acceptProposedAction();
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(this->closeApplication())
        e->accept();
    else
        e->ignore();
}

void MainWindow::disassembleFile(const QString &file)
{
    QHexEditData* hexeditdata = QHexEditData::fromFile(file);
    DisassemblerDialog ld(hexeditdata, this);

    if(!ld.hasLoaders())
    {
        QMessageBox m;
        m.setWindowTitle("No Loaders found...");
        m.setText("Cannot find a valid disassembler, do you want do load the file in binary mode?");
        m.setIcon(QMessageBox::Warning);
        m.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        m.setDefaultButton(QMessageBox::Yes);
        int ret = m.exec();

        switch(ret)
        {
            case QMessageBox::Yes:
                this->loadFile(file, hexeditdata);
                break;

            default:
                break;
        }

        return;
    }

    int res = ld.exec();

    if(res == DisassemblerDialog::Accepted && ld.selectedDisassembler())
    {
        DisassemblerView* dv = new DisassemblerView(ld.selectedDisassembler(), ld.canLoadDatabase(), hexeditdata, file, this->_lblinfo);

        this->setWindowTitle(QString("%1 [%2]").arg(this->_basetitle, QFileInfo(file).fileName()));
        this->setCentralWidget(dv);
        this->setSaveVisible(dv->canSave(), dv->canSaveAs());
        dv->disassemble();
    }
}

void MainWindow::loadFile(const QString &file)
{
    this->loadFile(file, QHexEditData::fromFile(file));
}

void MainWindow::checkCommandLine()
{
    QStringList args = qApp->arguments();    

    if(this->checkDisassembly(args))
        return;

    for(int i = 1; (i < args.length()) && (i < 10); i++)
    {
        QFileInfo fi(args[i]);

        if(!fi.isFile())
            break;

        this->loadFile(args[i]);
    }
}

bool MainWindow::checkDisassembly(const QStringList &args)
{
    if((args.count() < 3) || (QString::compare(args[1], "-disassemble", Qt::CaseInsensitive) != 0))
        return false;

    QFileInfo fi(args[2]);

    if(!fi.isFile())
        return false;

    this->disassembleFile(args[2]);
    return true;
}

bool MainWindow::closeApplication()
{
    if(qobject_cast<AbstractView*>(this->centralWidget()))
    {
        QMessageBox m;
        m.setWindowTitle("Closing...");
        m.setText("Do you want to close PREF?");
        m.setIcon(QMessageBox::Question);
        m.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        m.setDefaultButton(QMessageBox::No);
        int ret = m.exec();

        switch(ret)
        {
            case QMessageBox::Yes:
            {
                if(DebugDialog::instance()->isVisible())
                    DebugDialog::instance()->close();
                break;
            }

            default:
                return false;
        }
    }

    return true;
}

void MainWindow::setSaveVisible(bool cansave, bool cansaveas)
{
    ui->action_Save->setVisible(cansave);
    ui->action_Save_As->setVisible(cansaveas);
    ui->mainToolBar->actions()[2]->setVisible((cansave || cansaveas));
}

void MainWindow::centerWindowToScreen()
{
    /* Center Window */
    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
}

void MainWindow::loadFile(const QString &file, QHexEditData *hexeditdata)
{
    HexView* hv = new HexView(hexeditdata, file, this->_lblinfo);

    this->setWindowTitle(QString("%1 [%2]").arg(this->_basetitle, QFileInfo(file).fileName()));
    this->setCentralWidget(hv);
    this->setSaveVisible(hv->canSave(), hv->canSaveAs());
}

void MainWindow::on_action_Analyze_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Load a file...");

    if(!file.isEmpty())
        this->loadFile(file, QHexEditData::fromFile(file));
}

void MainWindow::on_action_Save_As_triggered()
{
    qobject_cast<AbstractView*>(this->centralWidget())->saveAs();
}

void MainWindow::on_action_Save_triggered()
{
    qobject_cast<AbstractView*>(this->centralWidget())->save();
}

void MainWindow::on_actionExit_triggered()
{
    if(this->closeApplication())
        qApp->exit();
}

void MainWindow::on_actionCompare_triggered()
{
    CompareDialog cd(this);
    int res = cd.exec();

    if(res == CompareDialog::Accepted)
    {
        QDir l(cd.leftCompare());
        QDir r(cd.rightCompare());
        CompareView* cv = new CompareView(cd.leftCompare(), cd.rightCompare(), this->_lblinfo);

        this->setWindowTitle(QString("%1 [%2]").arg(this->_basetitle, QString("Compare: %1 -> %2").arg(l.dirName(), r.dirName())));
        this->setCentralWidget(cv);
        this->setSaveVisible(cv->canSave(), cv->canSaveAs());
    }
}

void MainWindow::on_actionAbout_PREF_triggered()
{
    AboutDialog ad(this);
    ad.exec();
}

void MainWindow::on_actionSignature_DB_triggered()
{
    SignatureDatabaseDialog sdd(this);
    sdd.exec();
}

void MainWindow::on_actionHex_File_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Import Hex file...");

    if(!file.isEmpty())
    {
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        QByteArray ba = QByteArray::fromHex(f.readAll());
        f.close();

        HexView* hv = new HexView(QHexEditData::fromMemory(ba), file, this->_lblinfo);

        this->setWindowTitle(QString("%1 [%2]").arg(this->_basetitle, QFileInfo(file).fileName()));
        this->setCentralWidget(hv);
        this->setSaveVisible(hv->canSave(), hv->canSaveAs());
    }
}

void MainWindow::on_actionDisassemble_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Disassemble File...");

    if(!file.isEmpty())
        this->disassembleFile(file);
}
