#include "sqlitestatement.h"

namespace SQLite
{
    SQLiteStatement::SQLiteStatement(const SQLiteDatabase &db, string query): _db(db), _query(query), _stmt(nullptr)
    {
        if(sqlite3_prepare_v2(db, query.c_str(), query.length(), &this->_stmt, nullptr) != SQLiteResults::Ok)
            throw SQLiteException(db);
    }

    SQLiteStatement::~SQLiteStatement()
    {
        if(this->_stmt)
            sqlite3_finalize(this->_stmt);
    }

    bool SQLiteStatement::step() const
    {
        SQLiteResults::SQLiteResult res = static_cast<SQLiteResults::SQLiteResult>(sqlite3_step(this->_stmt));

        if(res != SQLiteResults::Done && res != SQLiteResults::Row)
            throw SQLiteException(this->_db);

        if(res == SQLiteResults::Row)
            return true;

        return false;
    }

    int SQLiteStatement::execute() const
    {
        SQLiteResults::SQLiteResult res = static_cast<SQLiteResults::SQLiteResult>(sqlite3_step(this->_stmt));

        switch(res)
        {
            case SQLiteResults::Done:
                return sqlite3_changes(this->_db);

            case SQLiteResults::Row:
                throw SQLiteException("SQLiteStatement::execute() does not expect results");

            default:
                break;
        }

        throw SQLiteException(this->_db);
    }

    sqlite_int64 SQLiteStatement::count() const
    {
        return this->column(0);
    }

    SQLiteStatement::Column SQLiteStatement::result() const
    {
        return this->column(0);
    }

    SQLiteStatement::Column SQLiteStatement::column(int idx) const
    {
        return Column(this->_stmt, idx);
    }

    void SQLiteStatement::bind(int idx, int value) const
    {
        if(sqlite3_bind_int(this->_stmt, idx, value) != SQLiteResults::Ok)
            throw SQLiteException(this->_db);
    }

    void SQLiteStatement::bind(int idx, sqlite3_int64 value) const
    {
        if(sqlite3_bind_int64(this->_stmt, idx, value) != SQLiteResults::Ok)
            throw SQLiteException(this->_db);
    }

    void SQLiteStatement::bind(int idx, const char *value) const
    {
        if(sqlite3_bind_text(this->_stmt, idx, value, -1, nullptr) != SQLiteResults::Ok)
            throw SQLiteException(this->_db);
    }

    void SQLiteStatement::bind(int idx, const void* value, int len, SQLiteStatement::BlobType blobmgmt) const
    {
        if(sqlite3_bind_blob(this->_stmt, idx, value, len, reinterpret_cast<sqlite3_destructor_type>(blobmgmt)) != SQLiteResults::Ok)
            throw SQLiteException(this->_db);
    }

    void SQLiteStatement::bind(string param, const void* value, int len, SQLiteStatement::BlobType blobmgmt) const
    {
        int idx = sqlite3_bind_parameter_index(this->_stmt, param.c_str());
        this->bind(idx, value, len, blobmgmt);
    }
}
