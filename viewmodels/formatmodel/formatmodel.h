#ifndef FORMATMODEL_H
#define FORMATMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/fielddatamodel/fielddatamodel.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/format/elements/structure.h"

using namespace PrefSDK;

class FormatModel : public FieldDataModel, public LuaCTable
{
    Q_OBJECT

    public:
        typedef QList<lua_Integer> OffsetList;
        typedef QHash<lua_Integer, Structure*> StructureMap;
        typedef QHash<QString, Structure*> StringMap;

    public:
        explicit FormatModel(lua_State *l, ByteBuffer* byteBuffer, QObject *parent = 0);
        void clearItems();
        Structure* addStructure(const QString& name);
        Structure* addStructure(const QString& name, lua_Integer offset);
        Structure *structure(int i);
        Structure* find(const QString& name);
        lua_Integer structureCount();
        ByteBuffer *byteBuffer();

    lua_private:
        LuaTable::Ptr luaAddStructure(lua_String name);
        LuaTable::Ptr luaAddStructure(lua_String name, lua_Integer offset);
        LuaTable::Ptr luaStructure(lua_Integer i);
        LuaTable::Ptr luaFind(lua_String name);

    protected:
        virtual void metaIndex(lua_State* l);

    private slots:
        void updateModelData(FormatElement *sender);
        void updateModelData(qint64 offset, qint64 length, QHexEditData::ActionType);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &child) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        lua_State* _state;
        OffsetList _offsetlist;
        StructureMap _structuremap;
        StringMap _stringmap;

    private:
        static FormatModel* _empty;
        QFont _monospacefont;
        QImage _icostruct;
        QImage _icofield;
        QImage _icobitfield;
        ByteBuffer* _bytebuffer;
};

#endif // FORMATMODEL_H
