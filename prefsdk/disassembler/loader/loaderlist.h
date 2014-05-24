#ifndef PREFSDK_LOADERLIST_H
#define PREFSDK_LOADERLIST_H

#include <QtCore>
#include <lua.hpp>
#include "qhexedit/qhexeditdata.h"
#include "debugdialog/debugdialog.h"

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

            class LoadedLoader
            {
                public:
                    class Segment
                    {
                        public:
                            Segment(const QString& name, int64_t startaddress, int64_t endaddress, int64_t baseaddress): _name(name), _startaddress(startaddress), _endaddress(endaddress), _baseaddress(baseaddress) { }
                            const QString& name() const { return this->_name; }
                            int64_t startAddress() const { return this->_startaddress; }
                            int64_t endAddress() const { return this->_endaddress; }
                            int64_t baseAddress() const { return this->_baseaddress; }

                        private:
                            QString _name;
                            int64_t _startaddress;
                            int64_t _endaddress;
                            int64_t _baseaddress;
                    };

                public:
                    LoadedLoader(): _id(nullptr) { }
                    LoadedLoader(LoaderList::LoaderId loaderid): _id(loaderid) { }
                    void addSegment(const QString& name, int64_t startaddress, int64_t endaddress, int64_t baseaddress) { this->_segments.append(Segment(name, startaddress, endaddress, baseaddress));  }
                    LoadedLoader::Segment& segment(int idx) { return this->_segments[idx]; }
                    int segmentsCount() const { return this->_segments.length(); }

                private:
                    LoaderList::LoaderId _id;
                    QList<LoadedLoader::Segment> _segments;
            };

        private:
            LoaderList();
            static void loadLoader(lua_State *l, const QString& dir);

        public:
            static void load(lua_State* l);
            static void registerLoader(const QString& name, const QString& author, const QString& version, LoaderList::LoaderId loaderid);
            static void addLoadedLoader(LoaderList::LoaderId loaderid, QHexEditData *hexeditdata);
            static LoaderList::Loader& loader(LoaderId loaderid);
            static LoaderList::LoadedLoader& loadedLoader(QHexEditData* hexeditdata);

        private:
            static QList<Loader> _loaders;
            static QHash<LoaderList::LoaderId, int> _loadermap;
            static QHash<QHexEditData*, LoaderList::LoadedLoader> _loadedloaders;

        private:
            static const QString LOADERS_DIR;
            static const QString LOADER_MAIN_FILE;
    };
}
#endif // PREFSDK_LOADERLIST_H
