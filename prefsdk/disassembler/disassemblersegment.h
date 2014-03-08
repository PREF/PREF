#ifndef PREFSDK_DISASSEMBLERSEGMENT_H
#define PREFSDK_DISASSEMBLERSEGMENT_H

#include "disassemblerrange.h"

namespace PrefSDK
{
    class DisassemblerSegment : public DisassemblerRange
    {
        public:
            enum SegmentType { Code, Data };

        public:
            DisassemblerSegment();
            DisassemblerSegment(lua_Integer baseaddress, lua_Integer start, lua_Integer end, QString name, DisassemblerSegment::SegmentType type);
            lua_Integer baseAddress() const;
            QString name() const;
            DisassemblerSegment::SegmentType type() const;

        public:
            static void declareGlobals(lua_State *l);

        private:
            QString _name;
            lua_Integer _baseaddress;
            DisassemblerSegment::SegmentType _type;
    };
}

#endif // PREFSDK_DISASSEMBLERSEGMENT_H
