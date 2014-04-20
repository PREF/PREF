#ifndef PREFSDK_DISASSEMBLERRANGE_H
#define PREFSDK_DISASSEMBLERRANGE_H

#include <lua.hpp>
#include <QtCore>

namespace PrefSDK
{
    class DisassemblerRange
    {
        public:
            DisassemblerRange();
            DisassemblerRange(quint64 start, quint64 end);
            DisassemblerRange(const DisassemblerRange& dr);
            quint64 start() const;
            quint64 end() const;
            quint64 size() const;
            bool contains(quint64 addr) const;
            bool isEmpty() const;
            DisassemblerRange &operator=(const DisassemblerRange& rhs);

        private:
            quint64 _start;
            quint64 _end;
    };
}
#endif // PREFSDK_DISASSEMBLERRANGE_H
