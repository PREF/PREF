#ifndef DISASSEMBLERLISTING_H
#define DISASSEMBLERLISTING_H

#include <algorithm>
#include <QtCore>
#include "prefsdk/disassembler/blocks/segment.h"

using namespace PrefSDK;

class DisassemblerListing : public QObject
{
    Q_OBJECT

    public:
        typedef QList<uint64_t> AddressList;
        typedef QHash<uint64_t, Segment*> SegmentList;

    public:
        explicit DisassemblerListing(QObject *parent = 0);
        void addSegment(Segment* segment);
        Segment* segment(int idx);
        int segmentsCount() const;

    private:
        AddressList _addresslist;
        SegmentList _segments;
};

#endif // DISASSEMBLERLISTING_H
