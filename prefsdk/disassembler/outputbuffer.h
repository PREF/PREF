#ifndef PREFSDK_OUTPUTBUFFER_H
#define PREFSDK_OUTPUTBUFFER_H

#include "prefsdk/qlua.h"
#include "referencetable.h"
#include "prefsdk/datatype.h"

namespace PrefSDK
{
    class OutputBuffer : public LuaCTable
    {
        public:
            typedef std::shared_ptr<OutputBuffer> Ptr;

        public:
            enum OutputValues { Value, Address };

        public:
            OutputBuffer(lua_State* l, ReferenceTable* reftable);
            QString buffer();

        public:
            static void declareGlobals(lua_State* l);

        lua_public:
            void outSymbol(lua_String s);
            void outValue(lua_Integer val, lua_Integer datatype, lua_Integer outval);
            void outRegister(lua_Integer regidx);
            void out(lua_String s);

        private:
            QString _buffer;
            ReferenceTable* _reftable;
    };
}

#endif // PREFSDK_OUTPUTBUFFER_H
