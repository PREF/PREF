#ifndef SQLITE_SQLITEEXCEPTION_H
#define SQLITE_SQLITEEXCEPTION_H

#include <exception>
#include <sqlite3.h>

using namespace std;

namespace SQLite
{
    class SQLiteException : public std::exception
    {
        public:
            SQLiteException(const char* msg);
            SQLiteException(sqlite3* sqldb);
            SQLiteException(sqlite3* sqldb, const char* tail);
            virtual const char* what() const noexcept;

        private:
            const char* _msg;
            sqlite3* _sqldb;
    };
}

#endif // SQLITE_SQLITEEXCEPTION_H
