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

    private:
        LogWidgetHightlighter* _highlighter;
};

#endif // LOGWIDGET_H
