#ifndef PREFSDK_SYMBOL_H
#define PREFSDK_SYMBOL_H

#include <QtCore>
#include <QtGui>
#include "prefsdk/datatype.h"

namespace PrefSDK
{
    class Symbol : public QObject
    {
        Q_OBJECT

        public:
            explicit Symbol(DataType::Type datatype, const QString& name, QObject *parent = 0);
            DataType::Type dataType() const;
            const QString& name() const;

        private:
            DataType::Type _datatype;
            QString _name;
    };
}

#endif // PREFSDK_SYMBOL_H
