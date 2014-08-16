#ifndef PREFSDK_DISASSEMBLERLISTING_H
#define PREFSDK_DISASSEMBLERLISTING_H

#include <algorithm>
#include <QtCore>
#include "prefsdk/datatype.h"
#include "prefsdk/disassembler/symbol.h"
#include "prefsdk/disassembler/blocks/segment.h"
#include "prefsdk/disassembler/references/reference.h"

using namespace PrefSDK;

class DisassemblerListing : public QObject
{
    Q_OBJECT

    public:
        typedef QList<Segment*> SegmentList;
        typedef QList<Function*> FunctionList;
        typedef QMap<uint64_t, Instruction*> InstructionMap;
        typedef QSet<Reference*> ReferenceSet;
        typedef QHash<uint64_t, ReferenceSet> ReferenceMap;

    public:
        explicit DisassemblerListing(QObject *parent = 0);

    public: /* Reference Methods */
        void addReference(uint64_t srcaddress, uint64_t destaddress, ReferenceTypes::Type referencetype);
        ReferenceSet references(uint64_t address) const;

    public: /* Symbol Methods */
        void setSymbol(uint64_t address, DataType::Type type, const QString& name);
        const SymbolTable& symbolTable() const;

    public: /* Segment Methods */
        void addSegment(const QString& name, SegmentTypes::Type segmenttype, uint64_t startaddress, uint64_t endaddress, uint64_t baseoffset);
        Segment* segment(int idx);
        Segment* segmentFromAddress(uint64_t address);
        int segmentsCount() const;

    public: /* Function Methods */
        void addEntryPoint(const QString& name, uint64_t address);
        bool addFunction(FunctionTypes::Type type, uint64_t address, const QString& name);
        int functionsCount() const;
        Function* function(int idx);

    public: /* Instruction Methods */
        Instruction* createInstruction(uint64_t address);
        Instruction* instructionFromAddress(uint64_t address);
        Instruction* nextInstruction(Instruction* instruction);
        Instruction* mergeInstructions(Instruction* instruction1, Instruction* instruction2, const QString& mnemonic, InstructionCategories::Category category, InstructionTypes::Type type);
        bool hasNextInstruction(Instruction* instruction);

    public: /* Disassembler Metods */
        bool hasMoreInstructions();
        void push(uint64_t address, ReferenceTypes::Type referencetype);
        uint64_t pop();

    private:
        static bool sortBlocks(Block* block1, Block* block2);

    private:
        QStack<uint64_t> _stack;
        SegmentList _segments;
        FunctionList _functions;
        InstructionMap _instructions;
        Instruction* _currentinstruction;
        ReferenceMap _references;
        SymbolTable _symboltable;
        uint64_t _currentaddress;
};

#endif // PREFSDK_DISASSEMBLERLISTING_H
