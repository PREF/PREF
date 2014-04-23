#ifndef FORMATLIST_H
#define FORMATLIST_H

#include <lua.hpp>
#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/categorymanager.h"
#include "prefsdk/format/formattree.h"
#include "prefsdk/disassembler/disassembler.h"
#include "debugdialog/debugdialog.h"

namespace PrefSDK
{
    class FormatList
    {
        public:
            typedef const char* FormatId;

            class Format
            {
                public:
                    class Option
                    {
                        public:
                            Option(): _idx(-1) { }
                            Option(int optionidx, const QString& name, const QString& description): _idx(optionidx), _name(name), _description(description) { }
                            int index() const { return this->_idx; }
                            const QString& name() const { return this->_name; }
                            const QString& description() const { return this->_description; }

                        private:
                            int _idx;
                            QString _name;
                            QString _description;
                    };

                public:
                    Format(): _candisassemble(false), _id(nullptr) { }
                    Format(const QString& name, const QString& category, const QString& author, const QString& version, FormatId id): _name(name), _category(category), _author(author), _version(version), _candisassemble(false), _id(id) { }
                    void addOption(int optionidx, const QString& name, const QString& description) { this->_options[optionidx] = Option(optionidx, name, description); }
                    void enableDisassembler() { this->_candisassemble = true; }
                    Option option(int idx) const { return this->_options[idx + 1]; }
                    ProcessorLoader& disassemblerLoader() { return this->_disassembler; }
                    int optionsCount() const { return this->_options.count(); }
                    const QString& name() const { return this->_name; }
                    const QString& category() const { return this->_category; }
                    const QString& author() const { return this->_author; }
                    const QString& version() const { return this->_version; }
                    bool canDisassemble() const { return this->_candisassemble; }
                    FormatList::FormatId id() const { return this->_id; }

                private:
                    QString _name;
                    QString _category;
                    QString _author;
                    QString _version;
                    bool _candisassemble;
                    FormatId _id;
                    ProcessorLoader _disassembler;
                    QHash<int, Format::Option> _options;
            };

            class LoadedFormat
            {
                public:
                    typedef QList<uint64_t> AddressList;
                    typedef QHash<uint64_t, uint64_t> AddressMap;

                public:
                    LoadedFormat(): _id(nullptr), _formattree(nullptr) { }
                    LoadedFormat(FormatList::FormatId formatid, FormatTree* formattree): _id(formatid), _formattree(formattree) { }
                    FormatList::FormatId id() const { return this->_id; }
                    FormatTree* tree() { return this->_formattree; }
                    void addAddress(uint64_t address) { return this->_addresslist.append(address); qSort(this->_addresslist.begin(), this->_addresslist.end()); }
                    uint64_t addressAt(uint64_t i) const { return this->_addresslist.at(i); }

                private:
                    AddressList _addresslist;
                    AddressMap _addressmap;
                    FormatList::FormatId _id;
                    FormatTree* _formattree;
            };

        private:
            FormatList();
            static void loadFormats(lua_State* l, const QString &dir);

        public:
            static void load(lua_State *l);
            static void registerFormat(const QString& name, const QString& category, const QString& author, const QString& version, FormatList::FormatId formatid);
            static void registerOption(FormatList::FormatId formatid, int optionidx, const QString& name, const QString &description);
            static void addLoadedFormat(FormatList::FormatId formatid, FormatTree* formattree, QHexEditData *hexeditdata);
            static void removeLoadedFormat(QHexEditData* hexeditdata);
            static FormatList::Format& format(int i);
            static FormatList::Format& formatFromId(FormatList::FormatId id);
            static FormatList::LoadedFormat& loadedFormat(QHexEditData* hexeditdata);
            static int length();

        private:
            static QList<Format> _formats;
            static QHash<FormatList::FormatId, int> _formatmap;
            static QHash<QHexEditData*, LoadedFormat> _loadedformats;

        private:
            static const QString FORMATS_DIR;
            static const QString FORMAT_MAIN_FILE;
    };
}

#endif // FORMATLIST_H
