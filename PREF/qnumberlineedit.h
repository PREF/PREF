#ifndef QNUMBERLINEEDIT_H
#define QNUMBERLINEEDIT_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class QNumberLineEdit : public QLineEdit
{
    Q_OBJECT

    public:
        enum InputType {None = 0, AsciiString = 1, UnicodeString = 2, HexString = 3};
        QByteArray convertedData();
        explicit QNumberLineEdit(QWidget *parent = 0);
        QNumberLineEdit::InputType inputType();
        void setInputType(QNumberLineEdit::InputType it);

    protected:
        virtual QValidator::State validate(QString &input, int &pos) const;

    private:
        InputType _inputtype;
        QRegExpValidator* _binvalidator;
        QRegExpValidator* _octvalidator;
        QRegExpValidator* _decvalidator;
        QRegExpValidator* _hexvalidator;
};

#endif // QNUMBERLINEEDIT_H
