#ifndef PREFSDK_FORMATTREE_H
#define PREFSDK_FORMATTREE_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "elements/structure.h"
#include "elements/fieldarray.h"
#include "elements/field.h"
#include "elements/bitfield.h"
#include "debugdialog/debugdialog.h"

namespace PrefSDK
{
    class FormatTree: public QObject
    {
        Q_OBJECT

        private:
            typedef QHash<uint64_t, Structure*> StructureMap;
            typedef QList<uint64_t> StructureOffsetList;

        public:
            explicit FormatTree(lua_State* l, QHexEditData* hexeditdata, QObject* parent = 0);
            bool isEmpty() const;
            Structure* addStructure(const QString& name);
            Structure* addStructure(const QString& name, uint64_t offset);
            Structure* structure(uint64_t i);
            uint64_t structureCount();
            FormatElement* elementFromPool(int64_t i, const FormatElement* parent = nullptr);
            FormatElement* elementFromPoolByUUID(const QUuid& uuid);

        private:
            QHexEditData* _hexeditdata;
            StructureOffsetList _structureoffsets;
            StructureMap _structuremap;
            ElementPool _elementpool;
            lua_State* _state;
    };
}

#endif // PREFSDK_FORMATTREE_H
