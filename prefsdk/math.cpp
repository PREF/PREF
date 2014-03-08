#include "math.h"

namespace PrefSDK
{
    OccurrenceList generateOccList(ByteBuffer *bytebuffer, qint64 start, qint64 size)
    {
        OccurrenceList occlist;

        for(int i = 0; i <= 0xFF; i++)
            occlist.append(0);

        if(start < (size / 2))
            start = 0;
        else if(start > bytebuffer->length() - (size / 2))
            start = bytebuffer->length() - (size / 2);
        else
            start -= size / 2;

        for(qint64 i = start; i < start + size; i++)
        {
            if(i >= static_cast<uint>(bytebuffer->length()))
                break;

            uchar b = bytebuffer->at(i);
            occlist[b] += 1;
        }

        return occlist;
    }

    qreal entropy(ByteBuffer *bytebuffer)
    {
        return entropy(bytebuffer, 0, bytebuffer->length());
    }

    qreal entropy(ByteBuffer *bytebuffer, qint64 start, qint64 size)
    {
        qreal e = 0.0;
        qint64 base = qMin(size, static_cast<qint64>(256));
        OccurrenceList occs = generateOccList(bytebuffer, start, size);

        for(int i = 0; i < occs.length(); i++)
        {
            if(occs[i])
            {
                qreal freq = static_cast<qreal>(occs[i]) / static_cast<qreal>(size);
                e += freq * (log(freq) / log(base));
            }
        }

        return -e;
    }

    qreal entropy(OccurrenceList occlist, qint64 size)
    {
        qreal e = 0.0;
        qint64 base = qMin(size, static_cast<qint64>(256));

        for(int i = 0; i < occlist.length(); i++)
        {
            if(occlist[i])
            {
                qreal freq = static_cast<qreal>(occlist[i]) / static_cast<qreal>(size);
                e += freq * (log(freq) / log(base));
            }
        }

        return -e;
    }
}
