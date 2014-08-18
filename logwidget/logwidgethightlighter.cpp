#include "logwidgethightlighter.h"

LogWidgetHightlighter::LogWidgetHightlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
    QFont f("Monospace", qApp->font().pointSize());
    f.setStyleHint(QFont::TypeWriter); /* Use monospace fonts! */

    this->_defaultformat.setFont(f);
}

void LogWidgetHightlighter::highlightBlock(const QString &text)
{
    this->setFormat(0, text.length(), this->_defaultformat);
}
