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

        public:
            explicit MemoryBuffer(QHexEditData* hexeditdata, DisassemblerListing* listing, Logger* logger, DataType::Type datatype, QObject *parent = 0);

        public slots:
            lua_Integer read(lua_Integer address, lua_Integer datatype);

        private:
            DataBuffer* _databuffer;
            DisassemblerListing* _listing;
            Logger* _logger;
            DataType::Type _datatype;
    };
}

#endif // PREFSDK_MEMORYBUFFER_H
