#ifndef SEGMENTTYPE_H
#define SEGMENTTYPE_H

#include <QObject>

class SegmentType : public QObject
{
    Q_OBJECT

    Q_ENUMS(Type)

    public:
        enum Type { Code, Data };

    private:
        explicit SegmentType(QObject *parent = 0);
};

#endif // SEGMENTTYPE_H
