#ifndef PREFSDK_LOADERLIST_H
#define PREFSDK_LOADERLIST_H

#include <QtCore>
#include <algorithm>
#include <lua.hpp>
#include "qhexedit/qhexeditdata.h"
#include "debugdialog/debugdialog.h"
#include "prefsdk/disassembler/blocks/segment.h"

namespace PrefSDK
{
    class LoaderList
    {
        public:
            typedef const char* LoaderId;

            class Loader
            {
                public:
                    Loader(): _id(nullptr) { }
                    Loader(const QString& name, const QString& author, const QString& version, LoaderId id): _name(name), _author(author), _version(version), _id(id) { }
                    const QString& name() const { return this->_name; }
                    const QString& author() const { return this->_author; }
                    const QString& version() const { return this->_version; }
                    LoaderList::LoaderId id() const { return this->_id; }

                private:
                    QString _name;
                    QString _author;
                    QString _version;
                    LoaderId _id;
            };

        private:
            LoaderList();
            static void loadLoader(lua_State *l, const QString& dir);

        public:
            static void load(lua_State* l);
            static void registerLoader(const QString& name, const QString& author, const QString& version, LoaderList::LoaderId loaderid);
            static LoaderList::Loader& loader(LoaderId loaderid);

        private:
            static QList<Loader> _loaders;
            static QHash<LoaderList::LoaderId, int> _loadermap;

        private:
            static const QString LOADERS_DIR;
            static const QString LOADER_MAIN_FILE;
    };
}
#endif // PREFSDK_LOADERLIST_H
