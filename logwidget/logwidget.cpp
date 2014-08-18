#include "logwidget.h"

LogWidget::LogWidget(QWidget *parent): QPlainTextEdit(parent)
{
    this->_highlighter = new LogWidgetHightlighter(this->document());

    this->setReadOnly(true);
}

void LogWidget::writeLine(const QString &text, LogWidget::LogLevel loglevel)
{
    QString line = text;

    switch(loglevel)
    {
        case LogWidget::Notice:
            line.prepend("NOTICE: ");
            break;

        case LogWidget::Warning:
            line.prepend("WARNING: ");
            break;

        case LogWidget::Error:
            line.prepend("ERROR: ");
            break;

        default:
            break;
    }

    this->write(line.append("\n"));
}

void LogWidget::write(const QString &text)
{
    this->appendPlainText(text);
}
