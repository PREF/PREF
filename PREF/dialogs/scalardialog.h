#ifndef SCALARDIALOG_H
#define SCALARDIALOG_H

#include <QDialog>
#include <qhexedit/document/qhexdocument.h>

namespace Ui {
class ScalarDialog;
}

class ScalarDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit ScalarDialog(QWidget *parent = 0);
        ~ScalarDialog();

    public:
        static integer_t getScalar(QWidget* parent = 0, const QString& title = QString(), const QString& description = QString());
        void setDescription(const QString& s);
        integer_t value() const;

    private slots:
        void on_lineEdit_textChanged(const QString &s);

    private:
        void validateFields() const;
        int guessBase() const;

    private:
        Ui::ScalarDialog *ui;
        integer_t _value;
};

#endif // SCALARDIALOG_H
