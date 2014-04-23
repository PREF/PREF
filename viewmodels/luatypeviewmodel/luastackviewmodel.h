#ifndef LUASTACKVIEWMODEL_H
#define LUASTACKVIEWMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <lua.hpp>

class LuaStackViewModel : public QAbstractItemModel
{
    Q_OBJECT

    private:
        typedef QPair<QString, QString> StackItem;
        typedef QList<StackItem> StackList;

    public:
        explicit LuaStackViewModel(lua_State* l, QObject *parent = 0);
        void updateTop();
        int tableLength(int idx);
        QString typeValue(int idx);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        void generateStackList();

    private:
        lua_State* _state;
        StackList _stacklist;
        int _top;
};

#endif // LUASTACKVIEWMODEL_H
