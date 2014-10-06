#ifndef PREFSDK_BITFIELD_H
#define PREFSDK_BITFIELD_H

#include "fieldelement.h"

namespace PrefSDK
{
    class BitField : public FieldElement
    {
        Q_OBJECT

        public:
            explicit BitField(int bitstart, int bitend, quint64 offset, DataType::Type datatype, const QString& name, const QUuid& parentid, AbstractTree* formattree, QObject *parent = 0);
            quint64 mask() const;
            int bitStart() const;
            int bitEnd() const;

        public: /* Overriden Methods */
            virtual FormatElement::Type elementType() const;
            virtual quint64 size() const;
            virtual bool isOverflowed() const;

        public slots:
            virtual QString displayName() const;
            virtual QString displayType() const;
            virtual QString displayValue() const;

        protected:
            virtual void pushValue(lua_State *l);

        private:
            static quint64 createMask(int bitstart, int bitend);

        private:
            int _bitstart;
            int _bitend;
            quint64 _mask;
    };

}

#endif // PREFSDK_BITFIELD_H
