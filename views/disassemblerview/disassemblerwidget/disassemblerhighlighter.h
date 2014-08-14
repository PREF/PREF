#ifndef DISASSEMBLERHIGHLIGHTER_H
#define DISASSEMBLERHIGHLIGHTER_H

#include <QtCore>
#include <QtGui>
#include "views/disassemblerview/disassemblerlisting.h"
#include "prefsdk/disassembler/listingobject.h"
#include "prefsdk/disassembler/instruction.h"

using namespace PrefSDK;

class DisassemblerHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    private:
        enum Construct
        {
            HexDigits,
        };

    public:
        explicit DisassemblerHighlighter(QTextDocument *parent, ListingObject* listingobj);
        explicit DisassemblerHighlighter(QTextDocument *parent, const DisassemblerListing::ReferenceSet& references);

    private:
        void generateHighlighters();
        void highlight(const QString& text, const QString &stringregex, const QTextCharFormat &charformat);
        void highlightSegment(const QString& text);
        void highlightFunction(const QString& text);
        void highlightComment(const QString& text);
        void highlightInstruction(const QString& text);
        void highlightJumpLabel(const QString& text);

    protected:
        virtual void highlightBlock(const QString &text);

    private:
        ListingObject* _listingobj;
        DisassemblerListing::ReferenceSet _references;
        QTextCharFormat _segmentformat;
        QTextCharFormat _segmentkwformat;
        QTextCharFormat _functionformat;
        QTextCharFormat _functionnameformat;
        QTextCharFormat _hexdigitsformat;
        QTextCharFormat _registersformat;
        QTextCharFormat _commentformat;
        QTextCharFormat _jumplabelformat;
};

#endif // DISASSEMBLERHIGHLIGHTER_H
