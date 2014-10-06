#include "loadedviews.h"

namespace PrefSDK
{
    LoadedViews* LoadedViews::_instance = nullptr;

    LoadedViews::LoadedViews(QObject *parent) :QObject(parent)
    {
    }

    void LoadedViews::addView(AbstractView *view)
    {
        this->_views[view->data()] = view;
    }

    void LoadedViews::removeView(AbstractView *view)
    {
        this->_views.remove(view->data());
    }

    AbstractView *LoadedViews::view(QHexEditData *hexeditdata)
    {
        if(this->_views.contains(hexeditdata))
            return this->_views[hexeditdata];

        return nullptr;
    }

    LoadedViews *LoadedViews::instance()
    {
        if(!LoadedViews::_instance)
            LoadedViews::_instance = new LoadedViews();

        return LoadedViews::_instance;
    }
}
