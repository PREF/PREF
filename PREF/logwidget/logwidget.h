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
        void log(const QString& text);
        void logline(const QString& text);
        void notice(const QString& text);
        void warning(const QString& text);
        void error(const QString& text);

    private:
        void logline(const QString& text, LogWidget::LogLevel loglevel);

    private:
        LogWidgetHightlighter* _highlighter;
};

#endif // LOGWIDGET_H
