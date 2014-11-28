#include "math.h"

namespace PrefSDK
{
    qreal logn(qint64 n, qint64 base)
    {
        return log(n) / log(base);
    }

    OccurrenceList generateOccList(QHexEditData *hexeditdata, qint64 start, qint64 size, volatile bool *cancontinue)
    {
        OccurrenceList occlist;

        for(int i = 0; i <= 0xFF; i++)
            occlist.append(0);

        if(start < (size / 2))
            start = 0;
        else if(start > hexeditdata->length() - (size / 2))
            start = hexeditdata->length() - (size / 2);
        else
            start -= size / 2;

        QHexEditDataReader reader(hexeditdata);

        for(qint64 i = start; i < start + size; i++)
        {
            if(cancontinue && ((*cancontinue) == false))
                return OccurrenceList();

            if(i >= static_cast<uint>(hexeditdata->length()))
                break;

            uchar b = reader.at(i);
            occlist[b] += 1;
        }

        return occlist;
    }

    qreal entropy(QHexEditData *hexeditdata)
    {
        return entropy(hexeditdata, 0, hexeditdata->length());
    }

    qreal entropy(QHexEditData *hexeditdata, qint64 start, qint64 size)
    {
        OccurrenceList occurences = generateOccList(hexeditdata, start, size);
        return entropy(occurences, size);
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
