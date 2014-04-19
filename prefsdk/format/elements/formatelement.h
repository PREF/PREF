#ifndef PREFSDK_FORMATELEMENT_H
#define PREFSDK_FORMATELEMENT_H

#include <cstdint>
#include <QtCore>
#include <lua.hpp>
#include "elementtype.h"
#include "qhexedit/qhexeditdata.h"
#include "debugdialog/debugdialog.h"

namespace PrefSDK
{
    class FormatElement;
    typedef QHash<QUuid, FormatElement*> ElementPool;

    class FormatElement : public QObject
    {
        Q_OBJECT

        public:
            explicit FormatElement(lua_State* l, uint64_t offset, const QString& name, const QUuid &parentid, ElementPool& elementpool, QHexEditData* hexeditdata, QObject *parent = 0);
            uint64_t offset() const;
            uint64_t endOffset() const;
            int base() const;
            const QString& name() const;
            QString info() const;
            FormatElement* parentElement() const;
            const QUuid& id() const;
            virtual bool isDynamic() const;
            virtual bool hasChildren() const;
            bool hasParent() const;
            bool containsOffset(uint64_t offset) const;
            void setDynamic(bool b);
            virtual void setBase(int b);
            virtual QString displayName() const;
            const ElementPool& elementPool() const;
            virtual void parseChildren();

        signals:
            void baseChanged(int b);

        public: /* Abstract Methods */
            virtual ElementType::Type elementType() const = 0;
            virtual uint64_t size() const = 0;
            virtual int indexOf(FormatElement* fe) const = 0;
            virtual QString displayType() const = 0;
            virtual QString displayValue() const = 0;

        private:
            uint64_t _offset;
            int _base;
            QString _name;
            QUuid _id;
            QUuid _parentid;

        protected:
            ElementPool& _elementpool;
            QHexEditData* _hexeditdata;
            lua_State* _state;
            bool _dynamic;
    };

}

#endif // PREFSDK_FORMATELEMENT_H
