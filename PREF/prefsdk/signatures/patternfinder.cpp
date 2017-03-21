#include "patternfinder.h"

namespace PrefSDK
{
    PatternFinder::PatternFinder(FileFilterModel *filefiltermodel): _filefiltermodel(filefiltermodel)
    {

    }

    bool PatternFinder::analyze()
    {
        this->analyze(this->_startpattern, this->_filefiltermodel->smallestFileIndex());
        return !this->_startpattern.isEmpty();
    }

    bool PatternFinder::analyze(QByteArray &sp)
    {
        this->_startpattern = sp;
        return this->analyze();
    }

    void PatternFinder::analyze(QByteArray &sp, int idx)
    {
        if(idx < 0)
            return;

        if(sp.isEmpty() && (idx == this->_filefiltermodel->smallestFileIndex()))
            sp = this->readFile(idx);
        else
        {
            int i;
            QByteArray data = this->readFile(idx, sp.length());

            for(i = 0; i < sp.length(); i++)
            {
                if(sp[i] != data[i])
                    break;
            }

            if(i != sp.length()) /* Copy only if start-pattern is changed */
                sp = data.mid(0, i);
        }

        this->analyze(sp, idx - 1);
    }

    QByteArray PatternFinder::readFile(int i, qint64 len)
    {
        QFileInfo fi = this->_filefiltermodel->file(i);
        QFile f(fi.absoluteFilePath());

        f.open(QIODevice::ReadOnly);
        QByteArray ba = (len != -1 ? f.read(len) : f.readAll());
        f.close();

        return ba;
    }

    const QByteArray& PatternFinder::startPattern() const
    {
        return this->_startpattern;
    }
}
