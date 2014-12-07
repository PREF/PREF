#ifndef PREFSDK_FORMATELEMENT_H
#define PREFSDK_FORMATELEMENT_H

#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/format/abstracttree.h"
#include "qhexedit/qhexeditdatareader.h"
#include "views/abstractview.h"
#include <cstdint>
#include <QtCore>
#include <QQmlEngine>
#include <QMainWindow>
#include <QApplication>
#include <QWindow>

namespace PrefSDK
{
    class FormatElement : public QObject
    {
        Q_OBJECT

        Q_ENUMS(Type)

        Q_PROPERTY(PrefSDK::FormatElement::Type elementType READ elementType)
        Q_PROPERTY(quint64 offset READ offset)
        Q_PROPERTY(quint64 endoffset READ endOffset)
        Q_PROPERTY(quint64 size READ size)
        Q_PROPERTY(QString name READ name)
        Q_PROPERTY(QString info READ info)

        public:
            enum Type { InvalidType, StructureType, FieldType, FieldArrayType, BitFieldType };

        public:
            explicit FormatElement(QObject *parent = 0); /* Needed in QML */
            explicit FormatElement(quint64 offset, const QString& name, const QUuid &parentid, AbstractTree* formattree, QObject *parent = 0);
            virtual FormatElement::Type elementType() const;
            virtual quint64 size() const;
            virtual int indexOf(FormatElement*) const;
            quint64 offset() const;
            quint64 endOffset() const;
            int base() const;
            const QString& name() const;
            QString info();
            FormatElement* parentElement() const;
            const QUuid& id() const;
            virtual bool isDynamic() const;
            virtual bool hasChildren() const;
            bool hasParent() const;
            bool containsOffset(quint64 offset) const;
            void setDynamic(bool b);
            virtual void setBase(int b);
            virtual void parseChildren();

        public slots:
            PrefSDK::FormatElement* dynamicInfo(const PrefSDK::QtLua::LuaFunction& infoproc);
            PrefSDK::FormatElement* dynamicParser(bool condition, const PrefSDK::QtLua::LuaFunction& parseproc);

        protected:
            virtual void pushValue(lua_State* l);
            Q_INVOKABLE virtual int metaIndex(lua_State* l, const QString& key);

        signals:
            void baseChanged(int b);

        public slots:
            virtual QString displayName() const;
            virtual QString displayType() const;
            virtual QString displayValue() const;
            virtual void dump(const QString& filename) const;

        private:
            static QMainWindow* findMainWindow();
            static LogWidget* getLogWidget();

        private:
            quint64 _offset;
            int _base;
            QString _name;
            QUuid _id;
            QUuid _parentid;

        protected:
            AbstractTree* _formattree;
            PrefSDK::QtLua::LuaFunction _infoprocedure;
            PrefSDK::QtLua::LuaFunction _parseprocedure;
            bool _dynamic;

        protected:
            static QMainWindow* _mainwindow;
    };

}

#endif // PREFSDK_FORMATELEMENT_H
