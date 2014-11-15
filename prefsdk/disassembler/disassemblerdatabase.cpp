#include "disassemblerdatabase.h"

namespace PrefSDK
{
    const QString DisassemblerDatabase::DB_FILE_EXTENSION = ".db";

    DisassemblerDatabase::DisassemblerDatabase(SymbolTable* symboltable, const QString& filename, QObject *parent): QObject(parent), _symboltable(symboltable), _filename(filename)
    {
    }

    bool DisassemblerDatabase::exists(const QString &filename)
    {
        QFile dbfile(DisassemblerDatabase::adjustFile(filename));
        return dbfile.exists() && (dbfile.size() > 0);
    }

    QString DisassemblerDatabase::adjustFile(const QString &filename)
    {
        QFileInfo fi(filename);
        QString dbfilename = QString(fi.baseName()).append(DisassemblerDatabase::DB_FILE_EXTENSION);
        return QString("%1%2%3").arg(fi.path(), QDir::separator(), dbfilename);
    }

    void DisassemblerDatabase::populateDatabase()
    {
        SQLiteStatement(this->_disassemblerdb, "CREATE TABLE SymbolTable (Address INTEGER PRIMARY KEY,"
                                                                         "Name TEXT NOT NULL,"
                                                                         "Type INTEGER NOT NULL,"
                                                                         "Size INTEGER NOT NULL,"
                                                                         "ReferenceCount INTEGER NOT NULL)").execute();

        SQLiteStatement(this->_disassemblerdb, "CREATE TABLE ReferenceTable (Address INTEGER NOT NULL,"
                                                                            "ReferenceAddress TEXT NOT NULL,"
                                                                            "FOREIGN KEY(Address) REFERENCES SymbolTable(Address))").execute();
    }

    void DisassemblerDatabase::saveSymbols()
    {
        SQLiteStatement(this->_disassemblerdb, "BEGIN").execute();
        SQLiteStatement s(this->_disassemblerdb, "INSERT INTO SymbolTable (Address, Name, Type, Size, ReferenceCount) VALUES (?, ?, ?, ?, ?)");

        for(SymbolTable::Iterator it = this->_symboltable->begin(); it != this->_symboltable->end(); it++)
        {
            Symbol* symbol = it.value();
            const QList<DataValue>& sources = symbol->sources();
            sqlite3_int64 address = symbol->startAddress().compatibleValue<sqlite3_int64>();

            s.bind(1, address);
            s.bind(2, symbol->name().toUtf8().constData());
            s.bind(3, static_cast<sqlite3_int64>(symbol->type()));
            s.bind(4, symbol->size().compatibleValue<sqlite3_int64>());
            s.bind(5, sources.count());

            s.step();
            s.reset();
        }

        SQLiteStatement(this->_disassemblerdb, "COMMIT").execute();
    }

    void DisassemblerDatabase::saveReferences()
    {
        SQLiteStatement(this->_disassemblerdb, "BEGIN").execute();
        SQLiteStatement s(this->_disassemblerdb, "INSERT INTO ReferenceTable (Address, ReferenceAddress) VALUES (?, ?)");

        for(SymbolTable::Iterator it = this->_symboltable->begin(); it != this->_symboltable->end(); it++)
        {
            Symbol* symbol = it.value();

            if(!symbol->hasSources())
                continue;

            sqlite3_int64 address = symbol->startAddress().compatibleValue<sqlite3_int64>();
            const QList<DataValue>& sources = symbol->sources();

            for(int i = 0; i < sources.count(); i++)
            {
                sqlite3_int64 sourceaddress = sources[i].compatibleValue<sqlite3_int64>();

                s.bind(1, address);
                s.bind(2, sourceaddress);
                s.step();
                s.reset();
            }
        }

        SQLiteStatement(this->_disassemblerdb, "COMMIT").execute();
    }

    void DisassemblerDatabase::save()
    {
        QString filename = DisassemblerDatabase::adjustFile(this->_filename);

        if(QFile::exists(filename))
            QFile::remove(filename);

        this->_disassemblerdb.open(filename.toUtf8().constData());
        this->populateDatabase();
        this->saveSymbols();
        this->saveReferences();
        this->_disassemblerdb.close();
    }

    void DisassemblerDatabase::load()
    {
        this->_disassemblerdb.open(this->_filename.toUtf8().constData());
        SQLiteStatement s(this->_disassemblerdb, "SELECT * FROM SymbolTable");

        while(s.step())
        {
            DataValue addressvalue = DataValue::create(s.column(0).integer64(), this->_symboltable->addressType());
            this->_symboltable->set(static_cast<Symbol::Type>(s.column(2).integer()), addressvalue, QString::fromUtf8(s.column(1).text()));

            if(s.column(4).integer64())
            {
                Symbol* symbol = this->_symboltable->get(addressvalue);
                SQLiteStatement refs(this->_disassemblerdb, "SELECT * FROM ReferenceTable WHERE Address = ?");
                refs.bind(1, s.column(0).integer64());

                while(refs.step())
                {
                    DataValue referencevalue = DataValue::create(refs.column(1).integer64(), this->_symboltable->addressType());
                    symbol->addSource(referencevalue);
                }
            }
        }

        this->_disassemblerdb.close();
    }
}
