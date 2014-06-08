#ifndef OPERAND_H
#define OPERAND_H

#include <QtCore>

namespace PrefSDK
{
    namespace OperandTypes
    {
        enum Type
        {
            Undefined,
            Register,
            Immediate,
            Address,
            Expression,
        };
    }

    class Operand : public QObject
    {
        Q_OBJECT

        public:
            explicit Operand(OperandTypes::Type type, QObject *parent = 0);
            OperandTypes::Type type() const;
            const QString& value() const;
            void setDisplayValue(const QString& val);

        private:
            OperandTypes::Type _type;
            QString _value;
    };
}
#endif // OPERAND_H
