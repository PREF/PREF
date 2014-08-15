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
        typedef QMap<uint64_t, Instruction*> InstructionMap;
        typedef QSet<Reference*> ReferenceSet;
        typedef QHash<uint64_t, ReferenceSet> ReferenceMap;

    public:
        explicit DisassemblerListing(QObject *parent = 0);
        void addSegment(const QString& name, SegmentTypes::Type segmenttype, uint64_t startaddress, uint64_t endaddress, uint64_t baseoffset);
        void addEntryPoint(const QString& name, uint64_t address);
        void addFunction(FunctionTypes::Type type, uint64_t address, const QString& name);
        Instruction* addInstruction(uint64_t address);
        void addReference(uint64_t srcaddress, uint64_t destaddress, ReferenceTypes::Type referencetype);
        void setSymbol(uint64_t address, DataType::Type type, const QString& name);
        Segment* segment(int idx);
        Segment* segmentFromAddress(uint64_t address);
        int segmentsCount() const;
        int functionsCount() const;
        Function* function(int idx);
        ReferenceSet references(uint64_t address) const;
        const SymbolTable& symbolTable() const;
        Instruction* mergeInstructions(Instruction* instruction1, Instruction* instruction2, const QString& mnemonic, InstructionCategories::Category category, InstructionTypes::Type type);

    public: /* Disassembler Metods */
        bool hasMoreInstructions();
        void push(uint64_t address, ReferenceTypes::Type referencetype);
        uint64_t pop();

    private:
        void populateFunctions();

    private:
        QStack<uint64_t> _stack;
        InstructionMap _instructions;
        Instruction* _currentinstruction;
        uint64_t _currentaddress;
        AddressList _addresslist;
        SegmentList _segments;
        ReferenceMap _references;
        SymbolTable _symboltable;
};

#endif // PREFSDK_DISASSEMBLERLISTING_H
