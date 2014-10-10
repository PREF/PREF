#include "disassemblerhighlighter.h"

DisassemblerHighlighter::DisassemblerHighlighter(QTextDocument *parent, Block* block): QSyntaxHighlighter(parent), _block(block)
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

    this->_symbollabelformat.setFontFamily("Monospace");
    this->_symbollabelformat.setFontStyleHint(QFont::TypeWriter);
    this->_symbollabelformat.setFontWeight(QFont::Bold);
    this->_symbollabelformat.setForeground(Qt::darkMagenta);
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
    QRegExp regex("[a-z]*[ ]*function [^.]+\\([^.]*\\)[^.]*$");
    int idx = text.indexOf(regex);

    while(idx >= 0)
    {
        int len = regex.matchedLength();
        this->setFormat(idx, len, this->_functionformat);
        idx = text.indexOf(regex, idx + len);
    }

    regex = QRegExp("[^. ]+\\([^.]*\\)");
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
    QRegExp regex("#[^#]*$");
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
    Instruction* instr = qobject_cast<Instruction*>(this->_block);

    QTextCharFormat charformat;
    charformat.setFontFamily("Monospace");
    charformat.setFontStyleHint(QFont::TypeWriter);        

    switch(instr->type())
    {
        case InstructionType::InterruptTrap:
            charformat.setForeground(Qt::darkYellow);
            break;

        case InstructionType::Privileged:
            charformat.setForeground(Qt::darkRed);
            break;

        case InstructionType::Nop:
            charformat.setForeground(Qt::lightGray);
            break;

        case InstructionType::Stop:
            charformat.setForeground(QColor::fromRgb(0x82, 0x22, 0x22));
            break;

        case InstructionType::Call:
            charformat.setForeground(QColor::fromRgb(0x80, 0x80, 0x00));
            break;

        case InstructionType::Jump:
            charformat.setForeground(QColor::fromRgb(0xDC, 0x14, 0x3C));
            break;

        case InstructionType::ConditionalCall:
            charformat.setForeground(QColor::fromRgb(0x32, 0xCD, 0x32));
            charformat.setFontWeight(QFont::Bold);
            break;

        case InstructionType::ConditionalJump:
            charformat.setForeground(Qt::red);
            charformat.setFontWeight(QFont::Bold);
            break;

        case InstructionType::Add:
        case InstructionType::Sub:
        case InstructionType::Mul:
        case InstructionType::Div:
        case InstructionType::Mod:
        case InstructionType::AddCarry:
        case InstructionType::SubCarry:
        case InstructionType::Asl:
        case InstructionType::Asr:
            charformat.setForeground(QColor::fromRgb(0xDA, 0x70, 0xD6));
            break;

        case InstructionType::And:
        case InstructionType::Or:
        case InstructionType::Xor:
        case InstructionType::Not:
        case InstructionType::Lsl:
        case InstructionType::Lsr:
        case InstructionType::Ror:
        case InstructionType::Rol:
        case InstructionType::RorCarry:
        case InstructionType::RolCarry:
            charformat.setForeground(QColor::fromRgb(0x7B, 0x68, 0xEE));
            break;

        case InstructionType::In:
        case InstructionType::Out:
            charformat.setForeground(QColor::fromRgb(0xDA, 0xA5, 0x20));
            break;

        default:
            charformat.setForeground(Qt::black);
            break;
    }

    if((instr->category() == InstructionCategory::LoadStore) && (instr->type() == InstructionType::Undefined))
        charformat.setForeground(QColor::fromRgb(0x00, 0x80, 0x80));

    QRegExp regex("[a-zA-Z0-9]+");
    int idx = text.indexOf(regex);

    this->setFormat(idx, regex.matchedLength(), charformat);
    this->highlightJumpLabel(text);
    this->highlightSymbol(text);
}

void DisassemblerHighlighter::highlightJumpLabel(const QString &text)
{
    QRegExp regex("j_[a-zA-Z0-9]+[\\:]*");
    int idx = text.indexOf(regex);

    if(idx == -1)
        return;

    this->setFormat(idx, regex.matchedLength(), this->_jumplabelformat);
}

void DisassemblerHighlighter::highlightSymbol(const QString &text)
{
    QRegExp regex("[ |,][a-zA-Z_][a-zA-Z0-9_]+[,]*");
    int idx = text.indexOf(regex);

    if(idx == -1)
        return;

    this->setFormat(idx, regex.matchedLength(), this->_symbollabelformat);
}

void DisassemblerHighlighter::highlightBlock(const QString &text)
{
    if(this->_block->blockType() == Block::SegmentBlock)
        this->highlightSegment(text);
    else if(this->_block->blockType() == Block::FunctionBlock)
        this->highlightFunction(text);
    else if(this->_block->blockType() == Block::InstructionBlock)
        this->highlightInstruction(text);
    else if(this->_block->blockType() == Block::ReferenceBlock)
        this->highlightJumpLabel(text);

    this->highlightComment(text);
    this->highlight(text, "\\b[0-9a-fA-F]+h\\b", this->_hexdigitsformat);
    this->highlight(text, "\\$[a-zA-Z0-9]+", this->_registersformat);
}
