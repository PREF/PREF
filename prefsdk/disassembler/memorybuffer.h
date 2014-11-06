#ifndef PREFSDK_MEMORYBUFFER_H
#define PREFSDK_MEMORYBUFFER_H

#include <QObject>
#include "logwidget/logger.h"
#include "prefsdk/libs/preflib/databuffer.h"
#include "prefsdk/disassembler/disassemblerlisting.h"

namespace PrefSDK
{
    class MemoryBuffer : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(lua_Integer baseaddress READ baseAddress)

        public:
            explicit MemoryBuffer(QHexEditData* hexeditdata, DisassemblerListing* listing, Logger* logger, const DataValue& baseaddress, DataType::Type addresstype, QObject *parent = 0);
            DataValue read(const DataValue& address, DataType::Type datatype);
            DataValue read(const DataValue& address);
            lua_Integer baseAddress() const;

        public slots:
            lua_Integer read(lua_Integer address, lua_Integer datatype);

        private:
            DataBuffer* _databuffer;
            DisassemblerListing* _listing;
            Logger* _logger;
            DataValue _baseaddress;
            DataType::Type _addresstype;
    };
}

#endif // PREFSDK_MEMORYBUFFER_H
