#ifndef PREFSDK_LISTINGOBJECT_H
#define PREFSDK_LISTINGOBJECT_H

#include <QtCore>

namespace PrefSDK
{
    namespace ListingTypes
    {
        enum Type
        {
            Segment,
            Function,
            Instruction,
        };
    }

    class ListingObject : public QObject
    {
        Q_OBJECT

        public:
            explicit ListingObject(QObject *parent = 0);
            void setParentObject(ListingObject* parentobj);
            void setSegmentName(const QString& name);
            ListingObject* parentObject();
            QString segmentName() const;

        public: /* Abstract Methods */
            virtual ListingTypes::Type objectType() const = 0;
            virtual QString displayAddress() const = 0;

        private:
            ListingObject* _parentobj;
            QString _segmentname;
    };
}

#endif // PREFSDK_LISTINGOBJECT_H
