#ifndef QNUMBERSPINBOX_H
#define QNUMBERSPINBOX_H

#include <QSpinBox>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>

class QNumberSpinBox : public QSpinBox
{
    Q_OBJECT

    public:
        explicit QNumberSpinBox(QWidget *parent = 0);
        virtual void setMaximum(int max);
        int base();
        void setBase(int b);

    private:
        void updateRegExp();

    protected:
        virtual QValidator::State validate(QString &input, int &pos) const;
        virtual int valueFromText(const QString &text) const;
        virtual QString textFromValue(int val) const;

    private:
        int _base;
        QRegExpValidator* _binvalidator;
        QRegExpValidator* _octvalidator;
        QRegExpValidator* _decvalidator;
        QRegExpValidator* _hexvalidator;
};

#endif // QNUMBERSPINBOX_H

