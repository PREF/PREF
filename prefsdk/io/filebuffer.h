#ifndef PREFSDK_FILEBUFFER_H
#define PREFSDK_FILEBUFFER_H

#include <QtCore>
#include "abstractbuffer.h"

namespace PrefSDK
{
    class FileBuffer : public AbstractBuffer
    {
        public:
            explicit FileBuffer(lua_State* l, QFile* file);
            virtual lua_Integer length() const;
            virtual ~FileBuffer();

        protected: /* Overriden Methods */
            virtual QIODevice* device();

        public: /* Overriden Methods */
            virtual void save();
            virtual void insert(qint64 pos, const QByteArray& ba);
            virtual void write(qint64 pos, qint64 len, const QByteArray& ba);
            virtual void remove(qint64 pos, qint64 len);
            virtual uchar at(qint64 i);
            virtual QByteArray read(qint64 pos, qint64 len);

        private:
            QFile* _file;
            QByteArray _data;
            QBuffer _buffer;
    };
}

#endif // PREFSDK_FILEBUFFER_H
