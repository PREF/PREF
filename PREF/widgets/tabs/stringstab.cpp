#include "stringstab.h"
#include "ui_stringstab.h"
#include "../../platform/stringsworker.h"

StringsTab::StringsTab(QWidget *parent) : QWidget(parent), ui(new Ui::StringsTab), _stringsmodel(NULL), _proxymodel(NULL)
{
    ui->setupUi(this);

}

void StringsTab::initialize(QHexEditData *hexeditdata)
{
    this->_stringsmodel = new StringsModel(hexeditdata, this);
    this->_proxymodel = new QSortFilterProxyModel(this);
    this->_proxymodel->setSourceModel(this->_stringsmodel);
    ui->tvStrings->setModel(this->_proxymodel);

    StringsWorker* stringsworker = new StringsWorker(this->_stringlist, hexeditdata, this);
    connect(stringsworker, &StringsWorker::searchEnable, ui->leFilter, &QLineEdit::setEnabled);

    connect(stringsworker, &StringsWorker::finished, [this, stringsworker]() {
        this->_stringsmodel->initialize(this->_stringlist);
        stringsworker->deleteLater();
    });

    stringsworker->start();
}

StringsTab::~StringsTab()
{
    delete ui;
}
