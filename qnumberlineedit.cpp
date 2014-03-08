#include "qnumberlineedit.h"

QNumberLineEdit::QNumberLineEdit(QWidget *parent): QLineEdit(parent)
{
    this->_inputtype = QNumberLineEdit::None; /* Internal Use */

    this->_hexvalidator = new QRegExpValidator(this);
    this->_hexvalidator->setRegExp(QRegExp("[0-9A-Fa-f]{1,255}"));

    this->setInputType(QNumberLineEdit::AsciiString); /* Ascii String by Default */
}

QNumberLineEdit::InputType QNumberLineEdit::inputType()
{
    return this->_inputtype;
}

void QNumberLineEdit::setInputType(QNumberLineEdit::InputType it)
{
    if(this->_inputtype != it)
    {
        if(this->_inputtype == QNumberLineEdit::HexString) /* Convert to Standard String */
            this->setText(QString(QByteArray::fromHex(this->text().toLatin1())));
        else /* Convert to Hex String */
            this->setText(QString(this->text().toLatin1().toHex()).toUpper());

        this->_inputtype = it;

        if(this->_inputtype == QNumberLineEdit::AsciiString || this->_inputtype == QNumberLineEdit::UnicodeString)
            this->setValidator(nullptr);
        else
            this->setValidator(this->_hexvalidator);
    }
}

QValidator::State QNumberLineEdit::validate(QString &input, int &pos) const
{        
    if(this->_inputtype == QNumberLineEdit::HexString)
        return this->_hexvalidator->validate(input, pos);

    return QValidator::Acceptable;
}

QByteArray QNumberLineEdit::convertedData()
{
    if(this->_inputtype == QNumberLineEdit::HexString)
        return QByteArray::fromHex(this->text().toLatin1());
    else if(this->_inputtype == QNumberLineEdit::UnicodeString)
    {
        QTextCodec* codec = QTextCodec::codecForName("UTF-16");
        QTextEncoder* encoder = codec->makeEncoder(QTextCodec::IgnoreHeader);
        return encoder->fromUnicode(this->text());
    }

    return this->text().toLatin1();
}
