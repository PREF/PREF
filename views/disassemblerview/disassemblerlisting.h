#ifndef PREFSDK_DISASSEMBLERLISTING_H
#define PREFSDK_DISASSEMBLERLISTING_H

#include <algorithm>
#include <QtCore>
#include "prefsdk/datatype.h"
#include "prefsdk/disassembler/blocks/segment.h"
#include "prefsdk/disassembler/references/reference.h"
#include "prefsdk/disassembler/symbols/symbol.h"

using namespace PrefSDK;

class DisassemblerListing : public QObject
{
    Q_OBJECT

    public:
        typedef QList<uint64_t> AddressList;
        typedef QHash<uint64_t, Segment*> SegmentList;
        typedef QSet<Reference*> ReferenceSet;
        typedef QHash<uint64_t, ReferenceSet> ReferenceMap;
        typedef QHash<uint64_t, Symbol*> SymbolTable;

    public:
        explicit DisassemblerListing(QObject *parent = 0);
        void addSegment(Segment* segment);
        void addReference(uint64_t srcaddress, uint64_t destaddress, ReferenceTypes::Type referencetype);
        void setSymbol(uint64_t address, DataType::Type type, const QString& name);
        Segment* segment(int idx);
        int segmentsCount() const;
        ReferenceSet references(uint64_t address) const;
        const DisassemblerListing::SymbolTable& symbolTable() const;

    private:
        AddressList _addresslist;
        SegmentList _segments;
        ReferenceMap _references;
        SymbolTable _symboltable;
};

#endif // PREFSDK_DISASSEMBLERLISTING_H
