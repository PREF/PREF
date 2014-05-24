#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "processorloader.h"
#include "loader/loaderlist.h"

namespace PrefSDK
{
    namespace InstructionFeatures
    {
        enum Features
        {
            Stop    = 0x0001,
            Call    = 0x0002,

            Change1 = 0x0004,
            Change2 = 0x0008,
            Change3 = 0x0010,
            Change4 = 0x00020,
            Change5 = 0x00040,
            Change6 = 0x00080,

            Use1    = 0x00100,
            Use2    = 0x00200,
            Use3    = 0x00400,
            Use4    = 0x00800,
            Use5    = 0x01000,
            Use6    = 0x02000,

            Jump    = 0x04000,
            Shift   = 0x08000,
        };
    }

    namespace OperandType
    {
        enum Type
        {
            Void,

            Register,
            Memory,
            Phrase,
            Displacement,
            Immediate,

            JumpFar,
            JumpNear,

            CallFar,
            CallNear,
        };
    }
}

#endif // DISASSEMBLER_H
