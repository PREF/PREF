#include "stringfinderhelper.h"

StringFinderHelper::StringFinderHelper(QObject *parent): QObject(parent)
{
    this->_cancontinue = true;
    this->_lastperc = 0;
}

void StringFinderHelper::updateProgressBar(ByteBuffer* bytebuffer, qreal newval)
{
    int perc = static_cast<int>((newval / static_cast<qreal>(bytebuffer->length()) * 100));

    if(perc != this->_lastperc)
    {
        this->_lastperc = perc;
        emit progressChanged(perc);
    }
}

void StringFinderHelper::run(ByteBuffer* bytebuffer, int minlength, int maxlength)
{
    QString s;
    qreal offset = 0;

    this->_cancontinue = true;
    emit progressChanged(0); /* Set Progressbar to 0% */

    while(this->_cancontinue && (static_cast<quint64>(offset) < static_cast<quint64>(bytebuffer->length())))
    {
        uchar ch = bytebuffer->at(offset);

        if(s.length() == maxlength)
        {
            emit stringFound(static_cast<qint64>(offset - s.length()), s);
            s.clear();
        }
        else if(isprint(ch))
        {
            s.append(ch);
        }
        else if(s.length() >= minlength)
        {
            emit stringFound(static_cast<qint64>(offset - s.length()), s);
            s.clear();
        }
        else
            s.clear();

        offset++;
        this->updateProgressBar(bytebuffer, offset);
    }

    emit progressChanged(100); /* Set Progressbar to 100% */
    emit finished();
}

void StringFinderHelper::stop()
{
    this->_cancontinue = false;
}
