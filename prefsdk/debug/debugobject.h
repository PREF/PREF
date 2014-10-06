#ifndef PREFSDK_DEBUGOBJECT_H
#define PREFSDK_DEBUGOBJECT_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/loadedviews.h"

namespace PrefSDK
{
    class DebugObject : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QHexEditData* buffer READ buffer)

        public:
            explicit DebugObject(QObject *parent = 0);

        public slots:
            QHexEditData* buffer();
            void bind(QHexEditData* hexeditdata);
            void unbind();
            void log(const QString& msg);
            void logline(const QString& msg);
            void notice(const QString& msg);
            void warning(const QString& msg);
            void error(const QString& msg);

        private:
            QHexEditData* _hexeditdata;
    };
}

#endif // PREFSDK_DEBUGOBJECT_H
