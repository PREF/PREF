#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"

namespace PrefSDK
{
    typedef QList<qint64> OccurrenceList;
    static constexpr qint64 ENTROPY_SIZE_DEFAULT = 32;

    OccurrenceList generateOccList(QHexEditData *hexeditdata, qint64 start, qint64 size = ENTROPY_SIZE_DEFAULT);
    qreal entropy(QHexEditData *hexeditdata, qint64 start, qint64 size = ENTROPY_SIZE_DEFAULT);
    qreal entropy(OccurrenceList occlist, qint64 size = ENTROPY_SIZE_DEFAULT);
    qreal entropy(QHexEditData *hexeditdata);
}

#endif // MATH_H
