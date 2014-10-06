#ifndef PREFSDK_INSTRUCTIONTYPE_H
#define PREFSDK_INSTRUCTIONTYPE_H

#include <QtCore>

namespace PrefSDK
{
    class InstructionType : public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

        public:
            enum Type { Undefined = 0x00000000, InterruptTrap = 0x10000000, Privileged      = 0x20000000, Nop             = 0x40000000, Stop     = 0x80000000,
                        Call      = 0x00000001, Jump          = 0x00000002, ConditionalCall = 0x00000003, ConditionalJump = 0x00000004,
                        Push      = 0x00000005, Pop           = 0x00000006,
                        Add       = 0x00000007, Sub           = 0x00000008, Mul             = 0x00000009, Div             = 0x0000001A, Mod      = 0x0000001B,
                        AddCarry  = 0x0000001C, SubCarry      = 0x0000001D, Asl             = 0x0000001E, Asr             = 0x0000001F,
                        And       = 0x00000020, Or            = 0x00000021, Xor             = 0x00000022, Not             = 0x00080023, Lsl      = 0x00000024,
                        Lsr       = 0x00000025, Rol           = 0x00000026, Ror             = 0x00000027, RolCarry        = 0x00000028, RorCarry = 0x00000029,
                        In        = 0x0000002A, Out           = 0x0000002B };

        private:
            explicit InstructionType(QObject *parent = 0);
    };
}

#endif // PREFSDK_INSTRUCTIONTYPE_H
