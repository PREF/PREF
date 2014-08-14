#include "disassemblerhighlighter.h"

DisassemblerHighlighter::DisassemblerHighlighter(QTextDocument *parent, ListingObject* listingobj): QSyntaxHighlighter(parent), _listingobj(listingobj)
{
    this->generateHighlighters();
}

DisassemblerHighlighter::DisassemblerHighlighter(QTextDocument *parent, const DisassemblerListing::ReferenceSet &references): QSyntaxHighlighter(parent), _listingobj(nullptr), _references(references)
{
    this->generateHighlighters();
}

void DisassemblerHighlighter::generateHighlighters()
{
    this->_segmentformat.setFontFamily("Monospace");
    this->_segmentformat.setFontStyleHint(QFont::TypeWriter);
    this->_segmentformat.setForeground(Qt::darkGreen);

    this->_segmentkwformat.setFontFamily("Monospace");
    this->_segmentkwformat.setFontStyleHint(QFont::TypeWriter);
    this->_segmentkwformat.setFontWeight(QFont::Bold);
    this->_segmentkwformat.setForeground(Qt::darkGreen);

    this->_functionformat.setFontFamily("Monospace");
    this->_functionformat.setFontStyleHint(QFont::TypeWriter);
    this->_functionformat.setFontWeight(QFont::Bold);
    this->_functionformat.setForeground(Qt::blue);

    this->_functionnameformat.setFontFamily("Monospace");
    this->_functionnameformat.setFontStyleHint(QFont::TypeWriter);
    this->_functionnameformat.setForeground(Qt::darkRed);

    this->_hexdigitsformat.setFontFamily("Monospace");
    this->_hexdigitsformat.setFontStyleHint(QFont::TypeWriter);
    this->_hexdigitsformat.setForeground(Qt::darkBlue);

    this->_registersformat.setFontFamily("Monospace");
    this->_registersformat.setFontStyleHint(QFont::TypeWriter);
    this->_registersformat.setFontWeight(QFont::Bold);
    this->_registersformat.setForeground(Qt::darkBlue);

    this->_commentformat.setFontFamily("Monospace");
    this->_commentformat.setFontStyleHint(QFont::TypeWriter);
    this->_commentformat.setFontItalic(true);
    this->_commentformat.setForeground(Qt::darkGreen);

    this->_jumplabelformat.setFontFamily("Monospace");
    this->_jumplabelformat.setFontStyleHint(QFont::TypeWriter);
    this->_jumplabelformat.setForeground(Qt::darkGray);
}

void DisassemblerHighlighter::highlight(const QString &text, const QString& stringregex, const QTextCharFormat& charformat)
{
    QRegExp regex(stringregex);
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, charformat);
        idx = text.indexOf(regex, idx + len);
    }
}

void DisassemblerHighlighter::highlightSegment(const QString &text)
{
    QRegExp regex("segment [^.]+$");
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, 7, this->_segmentkwformat);
        this->setFormat(idx + 7, len - 7, this->_segmentformat);
        idx = text.indexOf(regex, idx + len);
    }
}

void DisassemblerHighlighter::highlightFunction(const QString &text)
{
    QRegExp regex("[a-z]* function [^.]+\\(\\)[^.]+$");
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, this->_functionformat);
        idx = text.indexOf(regex, idx + len);
    }

    regex = QRegExp("[^. ]+\\(\\)");
    idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, this->_functionnameformat);
        idx = text.indexOf(regex, idx + len);
    }
}

void DisassemblerHighlighter::highlightComment(const QString &text)
{
    QRegExp regex("#[^.]+$");
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, this->_commentformat);
        idx = text.indexOf(regex, idx + len);
    }
}

void DisassemblerHighlighter::highlightInstruction(const QString &text)
{
    Instruction* instr = qobject_cast<Instruction*>(this->_listingobj);

    QTextCharFormat charformat;
    charformat.setFontFamily("Monospace");
    charformat.setFontStyleHint(QFont::TypeWriter);        

    switch(instr->type())
    {
        case InstructionTypes::InterruptTrap:
            charformat.setForeground(Qt::darkYellow);
            break;

        case InstructionTypes::Privileged:
            charformat.setForeground(Qt::darkRed);
            break;

        case InstructionTypes::Nop:
            charformat.setForeground(Qt::darkGray);
            break;

        case InstructionTypes::Stop:
            charformat.setForeground(QColor::fromRgb(0x82, 0x22, 0x22));
            break;

        case InstructionTypes::Call:
            charformat.setForeground(QColor::fromRgb(0x80, 0x80, 0x00));
            break;

        case InstructionTypes::Jump:
            charformat.setForeground(QColor::fromRgb(0xDC, 0x14, 0x3C));
            break;

        case InstructionTypes::ConditionalCall:
            charformat.setForeground(QColor::fromRgb(0x32, 0xCD, 0x32));
            charformat.setFontWeight(QFont::Bold);
            break;

        case InstructionTypes::ConditionalJump:
            charformat.setForeground(Qt::red);
            charformat.setFontWeight(QFont::Bold);
            break;

        case InstructionTypes::Add:
        case InstructionTypes::Sub:
        case InstructionTypes::Mul:
        case InstructionTypes::Div:
        case InstructionTypes::Mod:
        case InstructionTypes::AddCarry:
        case InstructionTypes::SubCarry:
        case InstructionTypes::Asl:
        case InstructionTypes::Asr:
            charformat.setForeground(QColor::fromRgb(0xDA, 0x70, 0xD6));
            break;

        case InstructionTypes::And:
        case InstructionTypes::Or:
        case InstructionTypes::Xor:
        case InstructionTypes::Not:
        case InstructionTypes::Lsl:
        case InstructionTypes::Lsr:
        case InstructionTypes::Ror:
        case InstructionTypes::Rol:
        case InstructionTypes::RorCarry:
        case InstructionTypes::RolCarry:
            charformat.setForeground(QColor::fromRgb(0x7B, 0x68, 0xEE));
            break;

        case InstructionTypes::In:
        case InstructionTypes::Out:
            charformat.setForeground(QColor::fromRgb(0xDA, 0xA5, 0x20));
            break;

        default:
            charformat.setForeground(Qt::black);
            break;
    }

    if((instr->category() == InstructionCategories::LoadStore) && (instr->type() == InstructionTypes::Undefined))
        charformat.setForeground(QColor::fromRgb(0x00, 0x80, 0x80));

    QRegExp regex("[a-zA-Z0-9]+");
    int idx = text.indexOf(regex);

    this->setFormat(idx, regex.matchedLength(), charformat);
}

void DisassemblerHighlighter::highlightJumpLabel(const QString &text)
{
    QRegExp regex("j_[a-zA-Z0-9]+\\:");
    int idx = text.indexOf(regex);

    this->setFormat(idx, regex.matchedLength(), this->_jumplabelformat);
}

void DisassemblerHighlighter::highlightBlock(const QString &text)
{
    if(this->_listingobj)
    {
        if(this->_listingobj->objectType() == ListingTypes::Segment)
            this->highlightSegment(text);
        else if(this->_listingobj->objectType() == ListingTypes::Function)
            this->highlightFunction(text);
        else
            this->highlightInstruction(text);
    }
    else if(!this->_references.isEmpty())
        this->highlightJumpLabel(text);

    this->highlightComment(text);
    this->highlight(text, "[0-9a-fA-F]+h", this->_hexdigitsformat);
    this->highlight(text, "\\$[a-zA-Z0-9]+", this->_registersformat);
}
