#ifndef SQLITE_SQLITEDATABASE_H
#define SQLITE_SQLITEDATABASE_H

#include <sqlite3.h>
#include "sqliteexception.h"
#include "sqlitetypes.h"

namespace SQLite
{
    class SQLiteDatabase
    {
        public:
            SQLiteDatabase();
            ~SQLiteDatabase();
            void open(const char* uri);
            void close();
            bool isLoaded() const;
            sqlite_int64 lastRowId() const;
            operator sqlite3*() const;

        public:
            static void initialize();
            static void finalize();

        private:
            SQLiteDatabase(const SQLiteDatabase&);
            SQLiteDatabase& operator =(const SQLiteDatabase&);

        private:
            sqlite3* _sqldb;
    };
}

#endif // SQLITE_SQLITEDATABASE_H
