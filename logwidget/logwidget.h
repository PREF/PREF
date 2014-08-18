#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "logwidgethightlighter.h"

class LogWidget : public QPlainTextEdit
{
    Q_OBJECT

    public:
        enum LogLevel { Nothing, Notice, Warning, Error };

    public:
        explicit LogWidget(QWidget *parent = 0);
        void write(const QString& text);
        void writeLine(const QString& text, LogWidget::LogLevel loglevel = LogWidget::Nothing);

    private slots:
        void doWrite(const QString& text);
        void doWriteLine(const QString& text, LogWidget::LogLevel loglevel = LogWidget::Nothing);

    signals:
        void writeRequested(QString);
        void writeLineRequested(QString, LogWidget::LogLevel);

    private:
        LogWidgetHightlighter* _highlighter;
};

Q_DECLARE_METATYPE(LogWidget::LogLevel)

#endif // LOGWIDGET_H
