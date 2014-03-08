#include "signaturedatabasedialog.h"
#include "ui_signaturedatabasedialog.h"

SignatureDatabaseDialog::SignatureDatabaseDialog(QWidget *parent): QDialog(parent), ui(new Ui::SignatureDatabaseDialog), _analyzedsamples(0)
{
    ui->setupUi(this);
    ui->sigpreviewHexEdit->setVisible(false);
    ui->sigpreviewHexEdit->setReadOnly(true);
    ui->patternHexEdit->setReadOnly(true);
    ui->hSplitter->setStretchFactor(1, 1);
    ui->vSplitter->setStretchFactor(1, 0);

    this->createToolbar();

    this->_signaturedbmodel = new SignatureDatabaseModel();
    this->_filefiltermodel = new FileFilterModel();

    ui->tvSignatures->setModel(this->_signaturedbmodel);
    ui->tvFiles->setModel(this->_filefiltermodel);

    connect(this->_toolbar, SIGNAL(startTriggered()), this, SLOT(analyzeFiles()));
    connect(ui->leFilter, SIGNAL(textChanged(QString)), this->_filefiltermodel, SLOT(updateFilter(QString)));

    this->displayDatabaseInfo();
    this->resizeColumns(this->_signaturedbmodel, ui->tvSignatures);
}

SignatureDatabaseDialog::~SignatureDatabaseDialog()
{
    delete ui;
}

void SignatureDatabaseDialog::createToolbar()
{
    this->_toolbar = new ElaborateToolBar();
    this->_toolbar->setElaborateEnabled(false);
    this->_toolbar->addSeparator();

    this->_actsavesignature = this->_toolbar->addAction(QIcon(":/signature_database/res/databaseadd.png"), "Save Signature");
    this->_actsavesignature->setEnabled(false);

    this->_actdeletesignature = this->_toolbar->addAction(QIcon(":/signature_database/res/databaserem.png"), "Delete Signature");
    this->_actdeletesignature->setEnabled(false);

    this->_actmarksignature = this->_toolbar->addAction(QIcon(":/signature_database/res/databasemark.png"), "Mark Signature");
    this->_actmarksignature->setEnabled(false);

    connect(this->_actsavesignature, SIGNAL(triggered()), this, SLOT(saveSignature()));
    connect(this->_actdeletesignature, SIGNAL(triggered()), this, SLOT(deleteSignature()));
    connect(this->_actmarksignature, SIGNAL(triggered()), this, SLOT(markSignature()));

    QVBoxLayout* vl = new QVBoxLayout();
    vl->setContentsMargins(0, 0, 0, 0);
    vl->addWidget(this->_toolbar);

    ui->tbContainer->setLayout(vl);
}

void SignatureDatabaseDialog::resizeColumns(QAbstractItemModel* model, QTreeView* view)
{
    for(int i = 0; i < model->columnCount(); i++)
        view->resizeColumnToContents(i);
}

void SignatureDatabaseDialog::displayDatabaseInfo()
{
    ui->teLog->clear();

    QString c = QString::number(SignatureDatabase::database()->signatureCount());
    ui->teLog->append(QString("<b>PREF Signature Database (Version %1, %2 Signatures Available)</b>").arg(SignatureDatabase::database()->versionString(), c));
}

void SignatureDatabaseDialog::analyzeFiles()
{
    bool patternfound;
    PatternFinder pf(this->_filefiltermodel);

    this->_startpattern.clear();
    ui->leSignatureName->clear();
    this->_analyzedsamples = this->_filefiltermodel->rowCount();

    if(this->_signaturedbmodel->isPatternMarked())
    {
        QByteArray data = SignatureDatabase::database()->signaturePattern(this->_signaturedbmodel->markedPatternId());
        patternfound = pf.analyze(data);
        ui->leSignatureName->setText(SignatureDatabase::database()->signatureName(this->_signaturedbmodel->markedPatternId()));
        this->_actsavesignature->setEnabled(true);
    }
    else
        patternfound = pf.analyze();

    ui->tabWidget->setCurrentIndex(1);

    if(patternfound)
    {
        this->_startpattern = pf.startPattern();
        this->_toolbar->elaborationCompleted();
        ui->teLog->append(QString("<b>Pattern Length: %1 %2</b>").arg(QString::number(this->_startpattern.length()), (!this->_startpattern.length() ? "Byte" : "Bytes")));
        ui->leSignatureName->setEnabled(true);

        ui->patternHexEdit->setData(QHexEditData::fromMemory(this->_startpattern));
    }
    else
    {
        ui->patternHexEdit->setData(nullptr);
        ui->teLog->append("<b>No Matches...</b>");
    }
}

void SignatureDatabaseDialog::on_tbFolder_clicked()
{
    QString s = QFileDialog::getExistingDirectory();

    if(!s.isEmpty())
    {
        ui->leFolder->setText(s);
        this->_filefiltermodel->setDirectory(s);
    }
}

void SignatureDatabaseDialog::on_leFolder_textChanged(const QString &text)
{
    QPalette p = ui->leFolder->palette();

    if(QDir(text).exists())
    {
        this->_toolbar->setElaborateEnabled(true);
        p.setColor(QPalette::Text, qApp->palette().color(QPalette::Text));
    }
    else
    {
        this->_toolbar->setElaborateEnabled(false);
        p.setColor(QPalette::Text, QColor(Qt::red));
    }

    ui->leFolder->setPalette(p);
}

void SignatureDatabaseDialog::saveSignature()
{
    ui->tabWidget->setCurrentIndex(1);

    QMessageBox mb;
    mb.setWindowTitle("Updating Signature DB");
    mb.setText("Do you want to save changes? Signatures with the same name will be overrided.\nPress 'Yes' to continue or 'No' to cancel.");
    mb.setIcon(QMessageBox::Warning);
    mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if(mb.exec() == QMessageBox::Yes)
    {
        SignatureDatabase::database()->updateDB(ui->leSignatureName->text().simplified(), this->_analyzedsamples, this->_startpattern);
        this->_signaturedbmodel->updateData();
        this->resizeColumns(this->_signaturedbmodel, ui->tvSignatures);
    }
}

void SignatureDatabaseDialog::deleteSignature()
{
    QItemSelectionModel* selectionmodel = ui->tvSignatures->selectionModel();
    QModelIndexList indexlist = selectionmodel->selectedIndexes();

    for(int i = 0; i < indexlist.length(); i++)
    {
        SignatureDatabase::database()->removeSignature(SignatureDatabase::database()->signatureId(indexlist[i].row()));
        this->_signaturedbmodel->updateData();
        this->resizeColumns(this->_signaturedbmodel, ui->tvSignatures);
    }
}

void SignatureDatabaseDialog::markSignature()
{
    QItemSelectionModel* selectionmodel = ui->tvSignatures->selectionModel();
    QModelIndexList indexlist = selectionmodel->selectedIndexes();

    if(!indexlist.isEmpty())
    {
        QModelIndex& mi = indexlist.first();
        this->_signaturedbmodel->markPattern(mi.row());
    }
}

void SignatureDatabaseDialog::on_leSignatureName_textEdited(const QString &text)
{
    this->_actsavesignature->setEnabled(!text.isEmpty());
}

void SignatureDatabaseDialog::on_tvSignatures_clicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        QByteArray signature = SignatureDatabase::database()->signaturePattern(index.internalId());
        ui->sigpreviewHexEdit->setData(QHexEditData::fromMemory(signature));
    }
    else
        ui->sigpreviewHexEdit->setData(nullptr);

    ui->sigpreviewHexEdit->setVisible(index.isValid());
    this->_actmarksignature->setEnabled(index.isValid());
    this->_actdeletesignature->setEnabled(index.isValid());
}

void SignatureDatabaseDialog::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
    {
        this->_signaturedbmodel->updateData();
        this->resizeColumns(this->_signaturedbmodel, ui->tvSignatures);
    }
}
