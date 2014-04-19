#ifndef PREFSDK_CATEGORYMANAGER_H
#define PREFSDK_CATEGORYMANAGER_H

#include <QtCore>
#include <memory>

namespace PrefSDK
{
    class CategoryManager
    {
        private:
            static const char* GLOBAL_CATEGORY;
            static const char* UNCATEGORIZED_CATEGORY;

        public:
            class Category
            {
                public:
                    Category(): _name("(All)"), _formatscount(0) { }
                    Category(QString cn): _name(cn), _formatscount(0) {}
                    QString name() const { return this->_name; }
                    int formatsCount() const { return this->_formatscount; }

                    void update(int formatidx)
                    {
                        this->_formatscount++;
                        this->_formatindexes.append(formatidx);
                    }

                    int globalFormatIndex(int localidx) const
                    {
                        if(!QString::compare(this->_name, CategoryManager::GLOBAL_CATEGORY))
                            return localidx;

                        return this->_formatindexes.at(localidx);
                    }

                private:
                    QString _name;
                    int _formatscount;
                    QList<int> _formatindexes;
            };

            typedef std::shared_ptr<Category> CategoryPtr;

        private:
            CategoryManager();
            static void addGlobalCategory();

        public:
            static void add(QString categoryname, int formatidx);
            static void clear();
            static int categoryCount();
            static CategoryPtr category(QString categoryname);
            static CategoryPtr category(int idx);
            static CategoryPtr globalCategory();

        private:
            static QMap<QString, CategoryPtr> _categories;
    };
}

#endif // PREFSDK_CATEGORYMANAGER_H
