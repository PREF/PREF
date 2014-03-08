#ifndef SQLITE_SQLITETRANSACTION_H
#define SQLITE_SQLITETRANSACTION_H

#include <sqlite3.h>
#include "sqlitedatabase.h"

namespace SQLite
{
    class SQLiteTransaction
    {
        public:
            SQLiteTransaction(const SQLiteDatabase& db);
            ~SQLiteTransaction();
            void commit();

        private:
            const SQLiteDatabase& _db;
            bool _commited;
    };
}

#endif // SQLITE_SQLITETRANSACTION_H
