#ifndef SQLITE_SQLITETYPES_H
#define SQLITE_SQLITETYPES_H

#include "sqliteexception.h"

namespace SQLite
{
    namespace SQLiteTypes
    {
        enum SQLiteType
        {
            Null    = 0,
            Integer = 1,
            Real    = 2,
            Text    = 3,
            Blob    = 4,
        };
    }

    namespace SQLiteResults
    {
        enum SQLiteResult
        {
            Ok            = SQLITE_OK,
            Error         = SQLITE_ERROR,
            Internal      = SQLITE_INTERNAL,
            Permission    = SQLITE_PERM,
            Abort         = SQLITE_ABORT,
            Busy          = SQLITE_BUSY,
            Locked        = SQLITE_LOCKED,
            NoMemory      = SQLITE_NOMEM,
            Interrupt     = SQLITE_INTERRUPT,
            IOError       = SQLITE_IOERR,
            Corrupt       = SQLITE_CORRUPT,
            NotFound      = SQLITE_NOTFOUND,
            Full          = SQLITE_FULL,
            CannotOpen    = SQLITE_CANTOPEN,
            Protocol      = SQLITE_PROTOCOL,
            Empty         = SQLITE_EMPTY,
            Schema        = SQLITE_SCHEMA,
            TooBig        = SQLITE_TOOBIG,
            Constraint    = SQLITE_CONSTRAINT,
            Mismatch      = SQLITE_MISMATCH,
            Misuse        = SQLITE_MISUSE,
            NoLFS         = SQLITE_NOLFS,
            Authorization = SQLITE_AUTH,
            Format        = SQLITE_FORMAT,
            Range         = SQLITE_RANGE,
            NotDB         = SQLITE_NOTADB,
            Notice        = SQLITE_NOTICE,
            Warning       = SQLITE_WARNING,
            Row           = SQLITE_ROW,
            Done          = SQLITE_DONE,
        };
    }
}

#endif // SQLITE_SQLITETYPES_H
