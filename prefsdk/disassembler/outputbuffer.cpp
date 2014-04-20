#include "outputbuffer.h"

namespace PrefSDK
{
    OutputBuffer::OutputBuffer(lua_State* l, ReferenceTable *reftable): LuaCTable(l, "OutputBuffer")
    {
        this->_reftable = reftable;

        this->exportMethod<void, OutputBuffer, lua_String>("outSymbol", &OutputBuffer::outSymbol);
        this->exportMethod<void, OutputBuffer, lua_Integer, lua_Integer, lua_Integer>("outValue", &OutputBuffer::outValue);
        this->exportMethod<void, OutputBuffer, lua_Integer>("outRegister", &OutputBuffer::outRegister);
        this->exportMethod<void, OutputBuffer, lua_String>("out", &OutputBuffer::out);
    }

    QString OutputBuffer::buffer()
    {
        return this->_buffer;
    }

    void OutputBuffer::declareGlobals(lua_State *l)
    {
        LuaTable::Ptr t = LuaTable::create(l);

        t->set("Value", OutputBuffer::Value);
        t->set("Address", OutputBuffer::Address);

        t->pushGlobal("OutputValues");
    }

    void OutputBuffer::outSymbol(lua_String s)
    {
        this->out(s);
    }

    void OutputBuffer::outValue(lua_Integer val, lua_Integer datatype, lua_Integer outval)
    {
        //DataType::Type dt = static_cast<DataType::Type>(datatype);
        OutputBuffer::OutputValues ov = static_cast<OutputBuffer::OutputValues>(outval);

        switch(ov)
        {
            case OutputBuffer::Address:
            {
                if(this->_reftable->isReference(val))
                {
                    ReferenceTable::Reference ref = this->_reftable->reference(val);

                    if(ref.Type == ReferenceTable::Code)
                        this->_buffer.append(QString("%1%2").arg(ref.Prefix, QString::number(val, 16).toUpper()));
                }
            }
            break;

            default:
                throw LuaException("OutValue: Unsupported Type");
                break;
        }
    }

    void OutputBuffer::outRegister(lua_Integer regidx)
    {
        //TODO: DA IMPLEMENTARE
    }

    void OutputBuffer::out(lua_String s)
    {
        this->_buffer.append(QString::fromLatin1(s));
    }
}
