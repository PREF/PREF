#ifndef PREFSDK_IOFILE_H
#define PREFSDK_IOFILE_H

#include <QFile>
#include <lua.hpp>

namespace PrefSDK
{
    class IOFile : public QFile
    {
        Q_OBJECT

        Q_PROPERTY(lua_Integer size READ size)

        public:
            Q_ENUMS(OpenMode)

        public:
            enum OpenMode { ReadOnly = 0x0001, WriteOnly = 0x0002, ReadWrite = ReadOnly | WriteOnly,
                            Append = 0x0004, Truncate = 0x0008 };

        public:
            explicit IOFile(const QString& filename, IOFile::OpenMode openmode);
            virtual ~IOFile();

        private:
            static QFile::OpenModeFlag adjustMode(IOFile::OpenMode openmode);

        public slots:
            void seek(lua_Integer offset);
            QByteArray readAll();
            QByteArray read(lua_Integer maxlen);
            void write(const QString& s);
            void write(const QByteArray& ba);
            void close();

    };
}

#endif // PREFSDK_IOFILE_H
