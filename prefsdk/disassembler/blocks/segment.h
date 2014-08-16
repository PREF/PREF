#ifndef PREFSDK_SEGMENT_H
#define PREFSDK_SEGMENT_H

#include <algorithm>
#include "block.h"
#include "function.h"

namespace PrefSDK
{
    namespace SegmentTypes
    {
        enum Type
        {
            Code,
            Data,
        };
    }

    class Segment : public Block
    {
        Q_OBJECT

        public:
            typedef QList<Function*> FunctionList;
            typedef QList<Function*> EntryPointList;

        public:
            explicit Segment(const QString& name, SegmentTypes::Type type, uint64_t startaddress, uint64_t size, uint64_t baseoffset, QObject* parent = 0);
            void addFunction(Function* f);
            bool containsFunction(uint64_t address);
            int indexOf(Function* f) const;
            Function* function(int idx);
            Function* entryPoint(int idx);
            int functionsCount() const;
            int entryPointsCount() const;
            uint64_t baseOffset() const;
            QString name() const;
            SegmentTypes::Type type() const;

        public: /* Overriden Methods */
            virtual ListingTypes::Type objectType() const;

        private:
            static bool sortFunctions(Function* function1, Function* function2);

        private:
            FunctionList _functions;
            EntryPointList _entrypoints;
            QString _name;
            uint64_t _baseoffset;
            SegmentTypes::Type _type;
    };
}

#endif // PREFSDK_SEGMENT_H
