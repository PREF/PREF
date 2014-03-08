#ifndef SQLITE_SQLITESTATEMENT_H
#define SQLITE_SQLITESTATEMENT_H

#include <string>
#include <sqlite3.h>
#include "sqliteexception.h"
#include "sqlitetypes.h"
#include "sqlitedatabase.h"

using namespace std;

namespace SQLite
{
    class SQLiteStatement
    {
        public:
            class Column
            {
                public:
                    Column(sqlite3_stmt* stmt, int idx): _stmt(stmt), _idx(idx) { }

                public:
                    int integer() const { return sqlite3_column_int(this->_stmt, this->_idx); }
                    sqlite_int64 integer64() const { return sqlite3_column_int64(this->_stmt, this->_idx); }
                    const char* text() const { return reinterpret_cast<const char*>(sqlite3_column_text(this->_stmt, this->_idx)); }
                    const void* blob() const { return sqlite3_column_blob(this->_stmt, this->_idx); }

                public:
                    operator int() const { return this->integer(); }
                    operator sqlite_int64() const { return this->integer64(); }
                    operator const char*() const { return this->text(); }
                    operator const void*() const { return this->blob(); }

                private:
                    sqlite3_stmt* _stmt;
                    int _idx;
            };

            enum BlobType { Static = 0, Transient = -1 };

        public:
            SQLiteStatement(const SQLite::SQLiteDatabase& db, string query);
            ~SQLiteStatement();
            bool step() const;
            int execute() const;

        public:
            sqlite_int64 count() const;
            SQLiteStatement::Column result() const;
            SQLiteStatement::Column column(int idx) const;
            void bind(int idx, int value) const;
            void bind(int idx, sqlite_int64 value) const;
            void bind(int idx, const char* value) const;
            void bind(int idx, const void* value, int len) const;
            void bind(int idx, const void* value, int len, SQLiteStatement::BlobType blobmgmt = SQLiteStatement::Static) const;
            void bind(string param, const void* value, int len, SQLiteStatement::BlobType blobmgmt = SQLiteStatement::Static) const;

            template<typename T> void bind(string param, T value) const
            {
                int idx = sqlite3_bind_parameter_index(this->_stmt, param.c_str());
                this->bind(idx, value);
            }

        private:
            const SQLite::SQLiteDatabase& _db;
            string _query;
            sqlite3_stmt* _stmt;
    };
}

#endif // SQLITE_SQLITESTATEMENT_H
