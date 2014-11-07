#ifndef PREFSDK_DISASSEMBLERLISTING_H
#define PREFSDK_DISASSEMBLERLISTING_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include "prefsdk/disassembler/blocks/segment.h"
#include "prefsdk/disassembler/blocks/label.h"
#include "prefsdk/disassembler/symbol/symboltable.h"
#include "prefsdk/disassembler/symbol/constanttable.h"
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include <algorithm>
#include <QtCore>

namespace PrefSDK
{
    class DisassemblerListing : public QObject
    {
        Q_OBJECT

        public:
            typedef QMap<DataValue, Segment*> SegmentMap;
            typedef QMap<DataValue, Function*> FunctionMap;
            typedef QMap<DataValue, Instruction*> InstructionMap;
            typedef QMap<DataValue, Label*> LabelMap;
            typedef QList<Block*> BlockList;
            typedef QSet<DataValue> StringSymbolSet;
            typedef QList<Function*> EntryPointList;

        public:
            explicit DisassemblerListing(QHexEditData* hexeditdata, QObject *parent = 0);
            bool isAddress(const DataValue& address) const;
            bool isDecoded(const DataValue& address) const;
            qint64 pointsToString(const DataValue& address) const;
            qint64 length() const;
            SymbolTable* symbolTable();
            ConstantTable* constantTable();
            QHexEditData* data();
            const BlockList& blocks();
            const DisassemblerListing::SegmentMap& segments() const;
            const DisassemblerListing::FunctionMap& functions() const;
            const DisassemblerListing::EntryPointList& entryPoints() const;
            void createLabel(const DataValue& destaddress, const DataValue &calleraddress, const QString& name);
            void createSegment(const QString &name, Segment::Type segmenttype, const DataValue &startaddress, const DataValue &size, const DataValue &baseoffset);
            void createFunction(const QString& name, FunctionType::Type functiontype, const DataValue& address, const DataValue &calleraddress);
            void createFunction(FunctionType::Type functiontype, const DataValue& address, const DataValue &calleraddress);
            void createFunction(const QString &name, FunctionType::Type functiontype, const DataValue& address);
            void addInstruction(Instruction *instruction);
            Segment* findSegment(Block* block);
            Segment* findSegment(const DataValue& address) const;
            Function* findFunction(Block* block);
            Function* findFunction(const DataValue& address);
            Instruction* findInstruction(const DataValue& address) const;
            Block* findBlock(const DataValue& address);

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
            SymbolTable* _symboltable;
            ConstantTable* _constanttable;
            EntryPointList _entrypoints;
            SegmentMap _segments;
            FunctionMap _functions;
            InstructionMap _instructions;
            LabelMap _labels;
            BlockList _blocks;
    };
}

#endif // PREFSDK_DISASSEMBLERLISTING_H
