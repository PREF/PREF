#include "logwidget.h"

LogWidget::LogWidget(QWidget *parent): QPlainTextEdit(parent)
{
    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter); /* Use monospace fonts! */

    this->setFont(f);
    this->setReadOnly(true);

    this->_highlighter = new LogWidgetHightlighter(this->document());
}

void LogWidget::logline(const QString &text, LogWidget::LogLevel loglevel)
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

    this->insertPlainText(line);
}

void LogWidget::log(const QString &text)
{
    this->insertPlainText(text);
}

void LogWidget::logline(const QString &text)
{
    this->logline(QString(text).append("\n"), LogWidget::Nothing);
}

void LogWidget::notice(const QString &text)
{
    this->logline(QString(text).append("\n"), LogWidget::Notice);
}

void LogWidget::warning(const QString &text)
{
    this->logline(QString(text).append("\n"), LogWidget::Warning);
}

void LogWidget::error(const QString &text)
{
    this->logline(QString(text).append("\n"), LogWidget::Error);
}
