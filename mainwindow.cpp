#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow* MainWindow::_instance = nullptr;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    MainWindow::_instance = this; /* Make this instance available outside */

    ui->setupUi(this);
    this->centerWindowToScreen();

    this->_lblinfo = new QLabel();
    ui->statusBar->addWidget(this->_lblinfo, 1);

    connect(ui->tabWidget, SIGNAL(fileDragged(QString)), this, SLOT(loadFile(QString)));
    lua_State* l = SDKManager::initializeLua();

    if(l)
    {
        DebugDialog::createInstance(l);        
        SDKManager::registerMessageHandler();
        this->_sdkloaded = SDKManager::loadSDK();

    }

    if(!l || !this->_sdkloaded)
    {
        QMessageBox m;
        m.setWindowTitle("Lua Initialization Failed");
        m.setText("Cannot initialize Lua Interpreter (SDK is missing?)");
        m.exec();
    }
    else
        this->checkCommandLine();
}

AbstractView *MainWindow::loadedView(QHexEditData *hexeditdata)
{
    return this->_loadedviews[hexeditdata];
}

bool MainWindow::sdkLoaded()
{
    return this->_sdkloaded;
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow *MainWindow::instance()
{
    return MainWindow::_instance;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(this->closeApplication())
        e->accept();
    else
        e->ignore();
}

void MainWindow::checkCommandLine()
{
    QStringList args = qApp->arguments();

    for(int i = 1; (i < args.length()) && (i < 10); i++)
    {
        QFileInfo fi(args[i]);

        if(!fi.isFile())
            break;

        this->loadFile(args[i], QHexEditData::fromFile(args[i]));
    }
}

bool MainWindow::closeApplication()
{
    if(ui->tabWidget->count() > 0)
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

void MainWindow::loadFile(QString file, QHexEditData *hexeditdata)
{
    QString viewname = QFileInfo(file).fileName();
    HexView* hv = new HexView(hexeditdata, viewname, this->_lblinfo, this->_loadedviews, ui->tabWidget);
    ui->tabWidget->addTab(hv, viewname);
}

void MainWindow::setSaveVisible(bool b)
{
    ui->action_Save->setVisible(b);
    ui->action_Save_As->setVisible(b);
}

void MainWindow::centerWindowToScreen()
{
    /* Center Window */
    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
}

void MainWindow::loadFile(QString file)
{
    this->loadFile(file, QHexEditData::fromFile(file));
}

void MainWindow::on_action_Analyze_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Load a file...");

    if(!file.isEmpty())
        this->loadFile(file, QHexEditData::fromFile(file));
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QMessageBox m;
    m.setWindowTitle("Closing Tab");
    m.setText("Do you want to close this page (Unsaved changes will be lost)?");
    m.setIcon(QMessageBox::Warning);
    m.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    m.setDefaultButton(QMessageBox::No);
    int ret = m.exec();

    switch(ret)
    {
        case QMessageBox::Yes:
        {
            if(ui->tabWidget->count() == 1)
            {
                this->_lblinfo->clear(); /* Clear Status Bar Information */
                this->setSaveVisible(false);
            }

            ui->tabWidget->widget(index)->deleteLater(); /* Shedule object for deletion */
            ui->tabWidget->removeTab(index);

            break;
        }

        default:
            break;
    }
}

void MainWindow::on_action_Save_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as...");

    if(!filename.isEmpty())
        ((HexView*)ui->tabWidget->currentWidget())->save(filename);
}

void MainWindow::on_action_Save_triggered()
{
    QMessageBox m;
    m.setWindowTitle("Overwriting file...");
    m.setText("Do you want to overwrite the original file?");
    m.setIcon(QMessageBox::Warning);
    m.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    m.setDefaultButton(QMessageBox::Cancel);
    int ret = m.exec();

    switch(ret)
    {
        case QMessageBox::Yes:
            dynamic_cast<HexView*>(ui->tabWidget->currentWidget())->save();
            break;

        case QMessageBox::No:
            this->on_action_Save_As_triggered();

        default:
            break;
    }
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
        QString viewname = QString("Compare: %1 -> %2").arg(l.dirName(), r.dirName());
        ui->tabWidget->addTab(new CompareView(cd.leftCompare(), cd.rightCompare(), viewname, this->_lblinfo, this->_loadedviews, ui->tabWidget), viewname);
    }
}

void MainWindow::on_actionAbout_PREF_triggered()
{
    AboutDialog ad(this);
    ad.exec();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == -1)
    {
        this->setSaveVisible(false);
        return;
    }

    AbstractView* av = dynamic_cast<AbstractView*>(ui->tabWidget->widget(index));
    av->updateStatusBar();
    this->setSaveVisible(av->canSave());
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

        QString viewname = QFileInfo(file).fileName();
        HexView* fv = new HexView(QHexEditData::fromMemory(ba), viewname, this->_lblinfo, this->_loadedviews, ui->tabWidget);
        ui->tabWidget->addTab(fv, viewname);
    }
}

void MainWindow::on_actionDisassemble_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Disassemble File...");

    if(!file.isEmpty())
    {
        QHexEditData* hexeditdata = QHexEditData::fromFile(file);
        DisassemblerDialog dd(hexeditdata, this);

        if(!dd.hasLoaders())
        {
            QMessageBox m;
            m.setWindowTitle("No Loaders found...");
            m.setText("Cannot find a valid loader, do you want do load the file in binary mode?");
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

        int res = dd.exec();

        if(res == DisassemblerDialog::Accepted && dd.selectedLoader())
        {
            QString viewname = QFileInfo(file).fileName();
            DisassemblerView* dv = new DisassemblerView(dd.selectedLoader(), hexeditdata, viewname, this->_lblinfo, this->_loadedviews, ui->tabWidget);
            ui->tabWidget->addTab(dv, viewname);
        }
    }
}
