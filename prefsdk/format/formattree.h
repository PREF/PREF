#ifndef PREFSDK_FORMATTREE_H
#define PREFSDK_FORMATTREE_H

#include <QtCore>
#include "logwidget/logger.h"
#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/libs/preflib/databuffer.h"
#include "qhexedit/qhexeditdata.h"
#include "elements/structure.h"
#include "elements/fieldarray.h"
#include "elements/field.h"
#include "elements/bitfield.h"
#include "abstracttree.h"

namespace PrefSDK
{
    class FormatTree: public AbstractTree
    {
        Q_OBJECT

        Q_PROPERTY(PrefSDK::DataBuffer* buffer READ buffer)

        private:
            typedef QHash<quint64, Structure*> StructureMap; /* Offset -> Structure* */
            typedef QHash<QString, quint64> StructureNames;  /* Name -> Offset */
            typedef QList<quint64> StructureOffsets;

        public:
            explicit FormatTree(QHexEditData* hexeditdata, Logger *logger, qint64 baseoffset, QObject* parent = 0);
            FormatElement* elementFromPool(qint64 i, FormatElement *parent = nullptr);
            quint64 structureCount();
            bool isEmpty() const;

        private:
            PrefSDK::DataBuffer* buffer();

        protected:
            Q_INVOKABLE int metaIndex(lua_State* l, const QString& key);

        public slots:
            PrefSDK::Structure* structure(const QString& name);
            PrefSDK::Structure* structure(quint64 i);
            PrefSDK::Structure* addStructure(const QString& name);
            PrefSDK::Structure* addStructure(const QString& name, lua_Integer offset);

        private:
            DataBuffer* _databuffer;
            StructureOffsets _structureoffsets;
            StructureNames _structurenames;
            StructureMap _structures;
            qint64 _baseoffset;
    };
}

#endif // PREFSDK_FORMATTREE_H
