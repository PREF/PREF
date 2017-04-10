#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QPlainTextEdit>

class LogWidget : public QPlainTextEdit
{
    Q_OBJECT

    public:
        explicit LogWidget(QWidget *parent = 0);

    public slots:
        void log(const QString& s);
};

#endif // LOGWIDGET_H
