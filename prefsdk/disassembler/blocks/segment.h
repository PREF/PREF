#ifndef PREFSDK_SEGMENT_H
#define PREFSDK_SEGMENT_H

#include <algorithm>
#include "block.h"
#include "function.h"

namespace PrefSDK
{
    class Segment : public Block
    {
        Q_OBJECT

        Q_ENUMS(Type)

        public:
            enum Type { Code, Data };

        public:
            explicit Segment(const QString& name, Segment::Type type, const DataValue& startaddress, const DataValue& size, const DataValue& baseoffset, QObject* parent = 0);
            const DataValue& baseOffset() const;
            QString name() const;
            Segment::Type type() const;

        public: /* Overriden Methods */
            virtual Block::Type blockType() const;

        private:
            DataValue _baseoffset;
            Segment::Type _type;
            QString _name;
    };
}

#endif // PREFSDK_SEGMENT_H
