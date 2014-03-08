#ifndef PREFSDK_DISASSEMBLERRANGE_H
#define PREFSDK_DISASSEMBLERRANGE_H

#include "prefsdk/qlua.h"

namespace PrefSDK
{
    class DisassemblerRange
    {
        public:
            DisassemblerRange();
            DisassemblerRange(lua_Integer start, lua_Integer end);
            DisassemblerRange(const DisassemblerRange& dr);
            lua_Integer start() const;
            lua_Integer end() const;
            bool contains(lua_Integer addr) const;
            bool isEmpty() const;
            lua_Integer size() const;
            DisassemblerRange &operator=(const DisassemblerRange& rhs);

        private:
            lua_Integer _start;
            lua_Integer _end;
    };
}
#endif // PREFSDK_DISASSEMBLERRANGE_H
