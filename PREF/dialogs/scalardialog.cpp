#include "scalardialog.h"
#include "ui_scalardialog.h"
#include <QPushButton>

ScalarDialog::ScalarDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ScalarDialog), _value(0)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ScalarDialog::accept);
}

ScalarDialog::~ScalarDialog()
{
    delete ui;
}

integer_t ScalarDialog::getScalar(QWidget *parent, const QString &title, const QString &description)
{
    ScalarDialog sd(parent);
    sd.setWindowTitle(title);
    sd.setDescription(description);

    if(sd.exec() == ScalarDialog::Accepted)
        return sd.value();

    return UINT64_MAX;
}

void ScalarDialog::setDescription(const QString &s)
{
    ui->lblTitle->setText(s);
}

integer_t ScalarDialog::value() const
{
    return this->_value;
}

int ScalarDialog::guessBase() const
{
    QString s = ui->lineEdit->text();

    if(s.startsWith("0x", Qt::CaseInsensitive))
        return 16;

    return 10;
}

void ScalarDialog::on_lineEdit_textChanged(const QString &s)
{
    bool ok = false;
    int base = this->guessBase();

    this->_value = ui->lineEdit->text().toULongLong(&ok, base);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok && s.length());
}
