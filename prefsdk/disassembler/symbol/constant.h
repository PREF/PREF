#ifndef PREFSDK_CONSTANT_H
#define PREFSDK_CONSTANT_H

#include "prefsdk/type/datavalue.h"
#include <QObject>

namespace PrefSDK
{
    class Constant : public QObject
    {
        Q_OBJECT

        public:
            explicit Constant(const QString& name, const DataValue& value, QObject *parent = 0);
            const QString& name() const;
            const DataValue& value() const;

        private:
            QString _name;
            DataValue _value;
    };
}

#endif // PREFSDK_CONSTANT_H
