#ifndef PREFSDK_LOADEDVIEWS_H
#define PREFSDK_LOADEDVIEWS_H

#include "views/abstractview.h"
#include "qhexedit/qhexeditdata.h"

namespace PrefSDK
{
    class LoadedViews : public QObject
    {
        Q_OBJECT

        private:
            explicit LoadedViews(QObject *parent = 0);

        public:
            void addView(AbstractView* view);
            void removeView(AbstractView* view);
            AbstractView* view(QHexEditData* hexeditdata);
            static LoadedViews* instance();

        private:
            static LoadedViews* _instance;
            QHash<QHexEditData*, AbstractView*> _views;
    };

}

#endif // PREFSDK_LOADEDVIEWS_H
