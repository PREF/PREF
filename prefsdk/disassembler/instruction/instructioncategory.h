#ifndef PREFSDK_INSTRUCTIONCATEGORY_H
#define PREFSDK_INSTRUCTIONCATEGORY_H

#include <QtCore>

namespace PrefSDK
{
    class InstructionCategory : public QObject
    {
        Q_OBJECT

        Q_ENUMS(Category)

        public:
            enum Category { Undefined, ControlFlow, StackManipulation, LoadStore, TestCompare, Arithmetic, Logical, IO, InterruptTrap, Privileged, NoOperation };

        private:
            explicit InstructionCategory(QObject *parent = 0);
    };
}

#endif // PREFSDK_INSTRUCTIONCATEGORY_H
