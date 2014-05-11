#include "signaturesworker.h"

SignaturesWorker::SignaturesWorker(QObject *parent): QThread(parent), _hexeditdata(nullptr)
{
}

void SignaturesWorker::setData(QHexEditData *hexeditdata)
{
    this->_hexeditdata = hexeditdata;
}

const SignaturesWorker::OffsetList &SignaturesWorker::offsetList() const
{
    return this->_offsetlist;
}

const SignaturesWorker::SignatureMap &SignaturesWorker::signatures() const
{
    return this->_signatures;
}

bool SignaturesWorker::step(qint64 depth, uchar b, qint64& sigid)
{
    if(this->_invalidpathcache.contains(depth))
    {
        InvalidByteSetCache& invalidbytes = this->_invalidpathcache[depth];

        if(invalidbytes.contains(b))
            return false;
    }

    if(this->_dectablecache.contains(depth))
    {
        SignatureIdCache& sigids = this->_dectablecache[depth];

        if(sigids.contains(b))
        {
            if(sigids[b] != -1)
                sigid = sigids[b];

            return true;
        }
    }

    qint64 newsigid = -1;
    bool stepdone = SignatureDatabase::database()->step(depth, b, newsigid);

    if(stepdone)
    {
        if(!this->_dectablecache.contains(depth))
            this->_dectablecache[depth] = SignatureIdCache();

        this->_dectablecache[depth][b] = sigid;

        if(newsigid != -1)
            sigid = newsigid;
    }
    else
    {
        if(!this->_invalidpathcache.contains(depth))
            this->_invalidpathcache[depth] = InvalidByteSetCache();

        this->_invalidpathcache[depth].insert(b);
    }

    return stepdone;
}

QByteArray SignaturesWorker::signaturePattern(qint64 sigid)
{
    if(this->_sigpatterncache.contains(sigid))
        return this->_sigpatterncache[sigid];

    QByteArray sigpattern = SignatureDatabase::database()->signaturePattern(sigid);
    this->_sigpatterncache[sigid] = sigpattern;
    return sigpattern;
}

QString SignaturesWorker::signatureName(qint64 sigid)
{
    if(this->_signamecache.contains(sigid))
        return this->_signamecache[sigid];

    QString signame = SignatureDatabase::database()->signatureName(sigid);
    this->_signamecache[sigid] = signame;
    return signame;
}

bool SignaturesWorker::isSignatureValid(const QByteArray &ba, qint64 sigid)
{
    QByteArray pattern = this->signaturePattern(sigid);

    if(pattern.length() != ba.length())
        return false;

    for(int i = 0; i < pattern.length(); i++)
    {
        if(ba[i] != pattern[i])
            return false;
    }

    return true;
}

void SignaturesWorker::run()
{
    if(!this->_hexeditdata)
        return;

    this->_offsetlist.clear();
    this->_signatures.clear();

    QByteArray foundsig;
    QHexEditDataReader reader(this->_hexeditdata);
    qint64 lastsigid = -1, depth = 0, i = 0;

    while(i < this->_hexeditdata->length())
    {
        uchar b = reader.at(i);
        bool stepdone = this->step(depth, b, lastsigid);

        if(!stepdone)
        {
            if(lastsigid != -1 && this->isSignatureValid(foundsig, lastsigid))
            {
                depth--;
                qint64 offset = (i - depth) - 1;

                this->_offsetlist.append(offset);
                this->_signatures[offset] = SignatureData(foundsig.length(), this->signatureName(lastsigid));

                lastsigid = -1;
                i += depth;
            }
            else
                i++;

            depth = 0;
            foundsig.clear();
            continue;
        }

        depth++;
        foundsig.append(b);
        i++;
    }

     /* Clear Cache */
    this->_sigpatterncache.clear();
    this->_signamecache.clear();
    this->_dectablecache.clear();
    this->_invalidpathcache.clear();

    qSort(this->_offsetlist.begin(), this->_offsetlist.end());
}
