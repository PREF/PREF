#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include "logwidgethightlighter.h"
#include <QPlainTextEdit>
#include <QGuiApplication>

class LogWidget : public QPlainTextEdit
{
    Q_OBJECT

    public:
        enum LogLevel { Nothing, Notice, Warning, Error };

    public:
        explicit LogWidget(QWidget *parent = 0);

    public slots:
        void write(const QString& text);
        void writeLine(const QString& text);
        void writeNotice(const QString& text);
        void writeWarning(const QString& text);
        void writeError(const QString& text);

    private:
        void writeLine(const QString& text, LogWidget::LogLevel loglevel);

    private:
        LogWidgetHightlighter* _highlighter;
};

#endif // LOGWIDGET_H
