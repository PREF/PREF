#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <QtCore>
#include "prefsdk/io/bytebuffer.h"

namespace PrefSDK
{
    typedef QList<qint64> OccurrenceList;
    static constexpr qint64 ENTROPY_SIZE_DEFAULT = 32;

    OccurrenceList generateOccList(ByteBuffer *bytebuffer, qint64 start, qint64 size = ENTROPY_SIZE_DEFAULT);
    qreal entropy(ByteBuffer *bytebuffer, qint64 start, qint64 size = ENTROPY_SIZE_DEFAULT);
    qreal entropy(OccurrenceList occlist, qint64 size = ENTROPY_SIZE_DEFAULT);
    qreal entropy(ByteBuffer *bytebuffer);
}

#endif // MATH_H
