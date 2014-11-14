#ifndef PREFSDK_DISASSEMBLERDATABASE_H
#define PREFSDK_DISASSEMBLERDATABASE_H

#include <QObject>
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/sqlitewrapper/sqlitedatabase.h"
#include "prefsdk/sqlitewrapper/sqlitestatement.h"

using namespace SQLite;

namespace PrefSDK
{
    class DisassemblerDatabase : public QObject
    {
        Q_OBJECT

        public:
            explicit DisassemblerDatabase(SymbolTable *symboltable, const QString& filename, QObject *parent = 0);
            static QString adjustFile(const QString& filename);
            static bool exists(const QString& filename);

        private:
            void populateDatabase();
            void saveSymbols();
            void saveReferences();

        public slots:
            void save();
            void load();

        private:
            SQLiteDatabase _disassemblerdb;
            SymbolTable* _symboltable;
            QString _filename;

        private:
            static const QString DB_FILE_EXTENSION;
    };
}

#endif // PREFSDK_DISASSEMBLERDATABASE_H
