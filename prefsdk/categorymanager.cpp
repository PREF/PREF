#include "categorymanager.h"

namespace PrefSDK
{
    const char* CategoryManager::GLOBAL_CATEGORY = "(All)";
    const char* CategoryManager::UNCATEGORIZED_CATEGORY = "(No Category)";
    QMap<QString, CategoryManager::CategoryPtr> CategoryManager::_categories;

    CategoryManager::CategoryManager()
    {

    }

    void CategoryManager::add(QString categoryname, int formatidx)
    {
        QString key;
        CategoryPtr c;

        if(categoryname.simplified().isEmpty())
            key = CategoryManager::UNCATEGORIZED_CATEGORY;
        else
            key = categoryname.toLower();

        if(!CategoryManager::_categories.contains(key))
        {
            c = CategoryPtr(new Category(categoryname));
            CategoryManager::_categories.insert(key, c);
        }
        else
            c = CategoryManager::_categories[key];

        c->update(formatidx);
    }

    void CategoryManager::addGlobalCategory(int formatscount)
    {
        if(!CategoryManager::_categories.contains(CategoryManager::GLOBAL_CATEGORY))
        {
            CategoryPtr c(new Category(formatscount));
            CategoryManager::_categories.insert(CategoryManager::GLOBAL_CATEGORY, c);
        }
    }

    void CategoryManager::clear()
    {
        CategoryManager::_categories.clear();
    }

    int CategoryManager::categoryCount()
    {
        return CategoryManager::_categories.count();
    }

    CategoryManager::CategoryPtr CategoryManager::category(QString categoryname)
    {
        return CategoryManager::_categories[categoryname.toLower()];
    }

    CategoryManager::CategoryPtr CategoryManager::category(int idx)
    {
        QList<CategoryPtr> categories = CategoryManager::_categories.values();
        return categories.at(idx);
    }

    CategoryManager::CategoryPtr CategoryManager::globalCategory()
    {
        return CategoryManager::_categories[CategoryManager::GLOBAL_CATEGORY];
    }
}
