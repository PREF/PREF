#include "sqliteexception.h"

namespace SQLite
{
    SQLiteException::SQLiteException(const char *msg): exception(), _msg(msg), _sqldb(nullptr)
    {

    }

    SQLiteException::SQLiteException(sqlite3 *sqldb): exception(), _msg(nullptr), _sqldb(sqldb)
    {

    }

    const char *SQLiteException::what() const noexcept
    {
        if(this->_msg)
            return this->_msg;
        else if(this->_sqldb)
            return sqlite3_errmsg(this->_sqldb);

        return nullptr;
    }
}
