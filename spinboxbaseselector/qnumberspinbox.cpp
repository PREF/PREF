#include "qnumberspinbox.h"

QNumberSpinBox::QNumberSpinBox(QWidget *parent): QSpinBox(parent)
{
    this->_base = 0;
    this->_binvalidator = new QRegExpValidator(this);
    this->_octvalidator = new QRegExpValidator(this);
    this->_decvalidator = new QRegExpValidator(this);
    this->_hexvalidator = new QRegExpValidator(this);

    this->setBase(12); /* Hexadecimal Base by Default */
}

void QNumberSpinBox::setMaximum(int max)
{
    QSpinBox::setMaximum(max);
    this->updateRegExp();
}

int QNumberSpinBox::base()
{
    return this->_base;
}

void QNumberSpinBox::setBase(int b)
{
    if(this->_base != b && (b == 2 || b == 8 || b == 10 || b == 16))
    {
        this->_base = b;
        this->updateRegExp();

        switch(this->_base)
        {
            case 2:
                this->lineEdit()->setValidator(this->_binvalidator);
                break;

            case 8:
                this->lineEdit()->setValidator(this->_octvalidator);
                break;

            case 16:
                this->lineEdit()->setValidator(this->_hexvalidator);
                break;

            default: /* case 10: */
                this->lineEdit()->setValidator(this->_decvalidator);
                break;
        }

        this->setValue(this->value()); /* Update Value's View */
    }
}

void QNumberSpinBox::updateRegExp()
{
    int maxlen = QString::number(this->maximum()).length();
    this->_binvalidator->setRegExp(QRegExp(QString("[0-1]{1,%1}").arg(maxlen)));
    this->_octvalidator->setRegExp(QRegExp(QString("[0-8]{1,%1}").arg(maxlen)));
    this->_decvalidator->setRegExp(QRegExp(QString("[0-9]{1,%1}").arg(maxlen)));
    this->_hexvalidator->setRegExp(QRegExp(QString("(0x)?[0-9A-Fa-f]{1,%1}").arg(maxlen)));
}

QValidator::State QNumberSpinBox::validate(QString &input, int &pos) const
{
    switch(this->_base)
    {
        case 2:
            return this->_binvalidator->validate(input, pos);

        case 8:
            return this->_octvalidator->validate(input, pos);

        case 16:
            return this->_hexvalidator->validate(input, pos);

        default:
            break;
    }

    return this->_decvalidator->validate(input, pos);
}

QString QNumberSpinBox::textFromValue(int val) const
{
    QString s = QString::number(val, this->_base);

    if(this->_base == 16)
        return s.toUpper(); /* Hex Values in UpperCase */

    return s;
}

int QNumberSpinBox::valueFromText(const QString &text) const
{
    return text.toInt(nullptr, this->_base);
}
