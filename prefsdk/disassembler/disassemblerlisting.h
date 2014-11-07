#ifndef PREFSDK_DISASSEMBLERLISTING_H
#define PREFSDK_DISASSEMBLERLISTING_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include "prefsdk/disassembler/blocks/segment.h"
#include "prefsdk/disassembler/blocks/label.h"
#include "prefsdk/disassembler/symbol/symboltable.h"
#include "prefsdk/disassembler/symbol/constanttable.h"
#include "prefsdk/disassembler/references/referencetable.h"
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include <algorithm>
#include <QtCore>

namespace PrefSDK
{
    class DisassemblerListing : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(PrefSDK::Function* firstfunction READ firstFunction)
        Q_PROPERTY(PrefSDK::Function* lastfunction READ lastFunction)
        Q_PROPERTY(PrefSDK::Block* firstblock READ firstBlock)
        Q_PROPERTY(PrefSDK::Block* lastblock READ lastBlock)

        public:
            typedef QMap<DataValue, Segment*> SegmentMap;
            typedef QMap<DataValue, Function*> FunctionMap;
            typedef QMap<DataValue, Instruction*> InstructionMap;
            typedef QMap<DataValue, Label*> LabelMap;
            typedef QList<Block*> BlockList;
            typedef QSet<DataValue> StringSymbolSet;
            typedef QSet<DataValue> VariableSet;
            typedef QList<Function*> EntryPointList;

        public:
            explicit DisassemblerListing(QHexEditData* hexeditdata, QObject *parent = 0);
            DataType::Type addressType() const;
            void setAddressType(DataType::Type addresstype);
            bool isAddress(const DataValue& address) const;
            bool isDecoded(const DataValue& address) const;
            qint64 pointsToString(const DataValue& address) const;
            void calcFunctionBounds();
            qint64 length() const;
            PrefSDK::Block* firstBlock();
            PrefSDK::Block* lastBlock();
            PrefSDK::Function* firstFunction();
            PrefSDK::Function* lastFunction();
            ReferenceTable* referenceTable();
            SymbolTable* symbolTable();
            ConstantTable* constantTable();
            QHexEditData* data();
            const BlockList& blocks();
            const DisassemblerListing::SegmentMap& segments() const;
            const DisassemblerListing::FunctionMap& functions() const;
            const DisassemblerListing::EntryPointList& entryPoints() const;
            const DisassemblerListing::InstructionMap& instructions() const;
            const DisassemblerListing::StringSymbolSet& strings() const;
            const DisassemblerListing::VariableSet& variables() const;
            void createReference(const DataValue& srcaddress, const DataValue &referencedby, Reference::Type referencetype, qint64 insertidx = -1);
            void createLabel(const DataValue& destaddress, Instruction* instruction, const QString& name);
            void createSegment(const QString &name, Segment::Type segmenttype, const DataValue &startaddress, const DataValue &size, const DataValue &baseoffset);
            void createFunction(const QString& name, FunctionType::Type functiontype, const DataValue& address);
            void createFunction(FunctionType::Type functiontype, const DataValue& startaddress);
            void addInstruction(Instruction *instruction);
            Segment* findSegment(Block* block);
            Segment* findSegment(const DataValue& address) const;
            Function* findFunction(Block* block);
            Function* findFunction(const DataValue& address);
            Instruction* findInstruction(const DataValue& address) const;
            Block* findBlock(const DataValue& address);

        public slots:
            bool hasNextBlock(QObject* b);
            QObject* nextBlock(QObject* b);
            QObject* nextFunction(QObject* f);
            QObject* firstInstruction(QObject* f);
            QObject* nextInstruction(QObject* i);

        public: /* Modified Binary Search O(log(n) + 2k) */
            qint64 indexOf(Block* block);
            qint64 indexOf(const DataValue& address, Block::Type blocktype = Block::InstructionBlock);

        private:
            void checkSort();
            void removeInstructions(Instruction* from, Instruction* to);
            static bool sortBlocks(Block* block1, Block* block2);

        private:
            bool _blocksorted;
            QHexEditData* _hexeditdata;
            ReferenceTable* _referencetable;
            SymbolTable* _symboltable;
            ConstantTable* _constanttable;
            EntryPointList _entrypoints;
            StringSymbolSet _stringsymbols;
            VariableSet _variables;
            SegmentMap _segments;
            FunctionMap _functions;
            InstructionMap _instructions;
            LabelMap _labels;
            BlockList _blocks;
    };
}

#endif // PREFSDK_DISASSEMBLERLISTING_H
