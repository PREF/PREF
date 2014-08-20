#include "logwidget.h"

LogWidget::LogWidget(QWidget *parent): QPlainTextEdit(parent)
{
    qRegisterMetaType<LogWidget::LogLevel>("LogWidget::LogLevel");

    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter); /* Use monospace fonts! */

    this->setFont(f);
    this->setReadOnly(true);

    this->_highlighter = new LogWidgetHightlighter(this->document());

    /* Safe call across threads */
    connect(this, SIGNAL(writeRequested(QString)), this, SLOT(doWrite(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(writeLineRequested(QString,LogWidget::LogLevel)), this, SLOT(doWriteLine(QString,LogWidget::LogLevel)), Qt::QueuedConnection);
}

void LogWidget::doWrite(const QString &text)
{
    this->insertPlainText(text);
}

void LogWidget::doWriteLine(const QString &text, LogWidget::LogLevel loglevel)
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

    this->doWrite(line.append("\n"));
}

void LogWidget::writeLine(const QString &text, LogWidget::LogLevel loglevel)
{
    emit writeLineRequested(text, loglevel);
}

void LogWidget::writeNotice(const QString &text)
{
    this->writeLine(text, LogWidget::Notice);
}

void LogWidget::writeWarning(const QString &text)
{
    this->writeLine(text, LogWidget::Warning);
}

void LogWidget::writeError(const QString &text)
{
    this->writeLine(text, LogWidget::Error);
}

void LogWidget::write(const QString &text)
{
    emit writeRequested(text);
}
