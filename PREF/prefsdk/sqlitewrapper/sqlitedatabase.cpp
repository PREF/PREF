#include "sqlitedatabase.h"

namespace SQLite
{
    SQLiteDatabase::SQLiteDatabase(): _sqldb(nullptr)
    {

    }

    SQLiteDatabase::~SQLiteDatabase()
    {
        this->close();
    }

    void SQLiteDatabase::open(const char* uri)
    {
        if(sqlite3_open(uri, &this->_sqldb) != SQLiteResults::Ok)
            throw SQLiteException(this->_sqldb);
    }

    void SQLiteDatabase::close()
    {
        if(this->_sqldb)
        {
            sqlite3_close(this->_sqldb);
            this->_sqldb = nullptr;
        }
    }

    bool SQLiteDatabase::isLoaded() const
    {
        return this->_sqldb != nullptr;
    }

    sqlite_int64 SQLiteDatabase::lastRowId() const
    {
        return sqlite3_last_insert_rowid(this->_sqldb);
    }

    SQLite::SQLiteDatabase::operator sqlite3 *() const
    {
        return this->_sqldb;
    }

    void SQLiteDatabase::initialize()
    {
        if(sqlite3_initialize() != SQLiteResults::Ok)
            throw SQLiteException("SQLiteSQLiteDatabase::initialize(): Cannot initialize SQLite");
    }

    void SQLiteDatabase::finalize()
    {
        if(sqlite3_shutdown() != SQLiteResults::Ok)
            throw SQLiteException("SQLiteSQLiteDatabase::finalize(): Cannot finalize SQLite");
    }

    SQLiteDatabase::SQLiteDatabase(const SQLiteDatabase &)
    {

    }

    SQLiteDatabase &SQLiteDatabase::operator =(const SQLiteDatabase &)
    {
        return *this;
    }
}
