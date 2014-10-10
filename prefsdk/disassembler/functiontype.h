#ifndef PREFSDK_FUNCTIONTYPE_H
#define PREFSDK_FUNCTIONTYPE_H

#include <QObject>

namespace PrefSDK
{
    class FunctionType : public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

        public:
            enum Type { NormalFunction = 0x00000000, EntryPointFunction = 0x10000000, ExportFunction = 0x00001000, ImportFunction = 0x00002000 };

        private:
            explicit FunctionType(QObject *parent = 0);
    };
}

#endif // PREFSDK_FUNCTIONTYPE_H
