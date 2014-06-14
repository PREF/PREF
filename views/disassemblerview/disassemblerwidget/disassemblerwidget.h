#ifndef DISASSEMBLERWIDGET_H
#define DISASSEMBLERWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/disassembler/listingobject.h"
#include "views/disassemblerview/disassemblerlisting.h"
#include "disassemblerhighlighter.h"

using namespace PrefSDK;

class DisassemblerWidget : public QPlainTextEdit
{
    Q_OBJECT

    private:
        class DisassemblerBlock
        {
            public:
                DisassemblerBlock(DisassemblerListing* listing, ListingObject* listingobj, QTextCursor& cursor): _cursor(cursor), _listingobj(listingobj), _listing(listing) { }
                virtual int print() = 0;

            protected:
                void printAddress()
                {
                    this->_cursor.insertText(QString("%1:%2 ").arg(this->_listingobj->segmentName(), this->_listingobj->displayAddress()));
                }

                virtual QString displayReferences(const QString& prefix, const DisassemblerListing::ReferenceSet& references) const
                {
                    if(references.isEmpty())
                        return QString();

                    QString s = QString("# %1: ").arg(prefix);

                    for(DisassemblerListing::ReferenceSet::ConstIterator it = references.begin(); it != references.end(); it++)
                    {
                        if(it != references.begin())
                            s.append(", ");

                        s.append(QString("%1").arg((*it)->address(), 8, 16, QLatin1Char('0')).toUpper());
                    }

                    return s;
                }

            protected:
                QTextCursor& _cursor;
                ListingObject* _listingobj;
                DisassemblerListing* _listing;
        };

        class SegmentBlock: public DisassemblerBlock
        {
            public:
                SegmentBlock(DisassemblerListing* listing, Segment* segment, QTextCursor& cursor): DisassemblerBlock(listing, segment, cursor), _segment(segment) { }

                virtual int print()
                {
                    QString startaddress = QString("%1").arg(this->_segment->startAddress(), 8, 16, QLatin1Char('0')).toUpper();
                    QString endaddress = QString("%1").arg(this->_segment->endAddress(), 8, 16, QLatin1Char('0')).toUpper();

                    this->printAddress();
                    this->_cursor.insertText(QString("segment '%1' (Start Address: %2h, End Address: %3h)\n\n").arg(this->_segment->name(), startaddress, endaddress));
                    return 2;
                }

            private:
                Segment* _segment;
        };

        class FunctionBlock: public DisassemblerBlock
        {
            public:
                FunctionBlock(DisassemblerListing* listing, Function* func, QTextCursor& cursor): DisassemblerBlock(listing, func, cursor), _function(func) { }

                ~FunctionBlock()
                {
                    this->_cursor.insertText("\n");
                }

                virtual int print()
                {
                    DisassemblerListing::ReferenceSet references = this->_listing->references(this->_function->startAddress());

                    this->printAddress();
                    this->_cursor.insertText(QString("  %1 %2()\t %3\n").arg(this->functionType(), this->_function->name(), this->displayReferences("Called by", references)));
                    return 2;
                }

            private:
                QString functionType() const
                {
                    switch(this->_function->type())
                    {
                        case FunctionTypes::EntryPoint:
                            return "entrypoint function";

                        case FunctionTypes::Import:
                            return "import function";

                        case FunctionTypes::Export:
                            return "export function";

                        default:
                            break;
                    }

                    return "function";
                }

            private:
                Function* _function;
        };

        class InstructionBlock: public DisassemblerBlock
        {
            public:
                InstructionBlock(DisassemblerListing* listing, Instruction* instr, QTextCursor& cursor): DisassemblerBlock(listing, instr, cursor), _instruction(instr) { }

                virtual int print()
                {
                    int l = this->printLabel();

                    QTextCharFormat charformat;

                    this->printAddress();
                    this->highlight(charformat);
                    this->_cursor.insertText(QString("        %1 ").arg(this->_instruction->mnemonic()), charformat);
                    this->_cursor.insertText(QString("%1\n").arg(this->_instruction->displayOperands()), QTextCharFormat());

                    return l + 1;
                }

            private:
                int printLabel()
                {
                    DisassemblerListing::ReferenceSet references = this->_listing->references(this->_instruction->address());

                    if(references.count() == 1 && ((*references.begin())->type() == ReferenceTypes::Jump || (*references.begin())->type() == ReferenceTypes::ConditionalJump))
                    {
                        QTextCharFormat charformat;
                        charformat.setForeground(QColor(Qt::darkGray));

                        QString address = QString("%1").arg(this->_instruction->address(), 8, 16, QLatin1Char('0')).toUpper();
                        this->_cursor.insertBlock();
                        this->printAddress();
                        this->_cursor.insertText(QString("    j_%1:\t%2\n").arg(address, this->displayReferences("Referenced by", references)), charformat);
                        return 2;
                    }

                    return 0;
                }

                void highlight(QTextCharFormat& charformat)
                {
                    switch(this->_instruction->type())
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
                }

            private:
                Instruction* _instruction;
                QColor _interrupttrap;
                QColor _nofeaturecolor;
                QColor _callcolor;
                QColor _stopcolor;
                QColor _jumpcolor;
        };

    public:
        explicit DisassemblerWidget(QWidget *parent = 0);
        void setListing(DisassemblerListing* listing);
        void gotoFunction(Function* func);

    private:
        void displayListing();

    private slots:
        void highlightCurrentLine();

    private:
        DisassemblerListing* _listing;
        DisassemblerHighlighter* _highlighter;
        QHash<Segment*, int> _segmentlines;
        QHash<Function*, int> _functionlines;
        QHash<Instruction*, int> _instructionlines;
};

#endif // DISASSEMBLERWIDGET_H
