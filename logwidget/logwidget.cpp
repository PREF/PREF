#include "logwidget.h"

LogWidget::LogWidget(QWidget *parent): QPlainTextEdit(parent)
{
    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter); /* Use monospace fonts! */

    this->setFont(f);
    this->setReadOnly(true);

    this->_highlighter = new LogWidgetHightlighter(this->document());
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

    this->insertPlainText(line);
}

void LogWidget::write(const QString &text)
{
    this->insertPlainText(text);
}

void LogWidget::writeLine(const QString &text)
{
    this->writeLine(QString(text).append("\n"), LogWidget::Nothing);
}

void LogWidget::writeNotice(const QString &text)
{
    this->writeLine(QString(text).append("\n"), LogWidget::Notice);
}

void LogWidget::writeWarning(const QString &text)
{
    this->writeLine(QString(text).append("\n"), LogWidget::Warning);
}

void LogWidget::writeError(const QString &text)
{
    this->writeLine(QString(text).append("\n"), LogWidget::Error);
}
