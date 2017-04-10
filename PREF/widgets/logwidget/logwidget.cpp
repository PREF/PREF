#include "logwidget.h"
#include "loghighlighter.h"
#include <QGuiApplication>

#ifdef Q_OS_MAC
    #define MONOSPACE_FONT "Courier"
#else
    #define MONOSPACE_FONT "Monospace"
#endif

LogWidget::LogWidget(QWidget *parent) : QPlainTextEdit(parent)
{
    this->setReadOnly(true);
    this->setFont(QFont(MONOSPACE_FONT, qApp->font().pointSize()));
}

void LogWidget::log(const QString &s)
{
    this->insertPlainText(s);
}
