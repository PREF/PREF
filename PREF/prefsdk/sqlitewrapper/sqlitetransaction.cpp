#include "sqlitetransaction.h"

namespace SQLite
{
    SQLiteTransaction::SQLiteTransaction(const SQLiteDatabase &db): _db(db), _commited(false)
    {
        sqlite3_exec(this->_db, "BEGIN", nullptr,  nullptr, nullptr);
    }

    SQLiteTransaction::~SQLiteTransaction()
    {
        if(!this->_commited)
        {
            if(sqlite3_exec(this->_db, "ROLLBACK", nullptr, nullptr, nullptr) != SQLiteResults::Ok)
                throw SQLiteException(this->_db);
        }
    }

    void SQLiteTransaction::commit()
    {
        if(!this->_commited)
        {
            if(sqlite3_exec(this->_db, "COMMIT", nullptr, nullptr, nullptr) != SQLiteResults::Ok)
                throw SQLiteException(this->_db);

            this->_commited = true;
            return;
        }

        throw SQLiteException("SQLiteTransaction::commit(): Transaction already commited");
    }
}
