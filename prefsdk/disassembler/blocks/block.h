#ifndef PREFSDK_BLOCK_H
#define PREFSDK_BLOCK_H

#include "prefsdk/type/datavalue.h"
#include "prefsdk/luastate.h"

namespace PrefSDK
{
    class Block: public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

        public:
            enum Type { UnknownBlock, InstructionBlock, FunctionBlock, SegmentBlock, LabelBlock, SymbolBlock };

        public:
            explicit Block(QObject* parent = 0);
            explicit Block(const DataValue& startaddress, QObject* parent = 0);
            explicit Block(const DataValue& startaddress, const DataValue& size, QObject* parent = 0);
            void addSource(const DataValue& srcaddress);
            const QList<DataValue>& sources() const;
            bool hasSources() const;
            bool isBookmarked() const;
            void setBookmarked(bool b);
            void setEndAddress(const DataValue& address);
            DataValue endAddress() const;
            const DataValue& startAddress() const;
            const DataValue& size() const;
            bool contains(const DataValue &address) const;
            bool isEmpty() const;

        public:
            virtual Block::Type blockType() const;

        protected:
            DataValue _startaddress;
            DataValue _size;
            QList<DataValue> _sources;
            bool _boomarked;
    };
}
#endif // PREFSDK_BLOCK_H
