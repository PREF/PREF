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

void SignaturesWorker::run()
{
    if(!this->_hexeditdata)
        return;

    QByteArray foundsig;
    QHexEditDataReader reader(this->_hexeditdata);
    qint64 sigid, depth = 0, offset = 0, endoffset = this->_hexeditdata->length();

    while(offset < endoffset)
    {
        bool stepdone = SignatureDatabase::database()->step(reader.at(offset), depth, sigid, foundsig);

        /* We have found a good byte, check if it is a dead path */
        if(stepdone && !SignatureDatabase::database()->canContinue(reader.at(offset + 1), depth))
        {
            depth = 0;
            foundsig.clear();
            offset++;
            continue;
        }

        /* We have completed a path, verify depth and if it is valid, highlight it in QHexEdit */
        if(!stepdone && (depth > 0) && (offset < endoffset) && !SignatureDatabase::database()->canContinue(reader.at(offset + 1), depth))
        {
            /* Check False Positives: ignore it and continue the analysis */
            if(depth != SignatureDatabase::database()->signatureMaxDepth(sigid) || !SignatureDatabase::database()->isSignatureValid(foundsig, sigid))
            {
                depth = 0;
                foundsig.clear();
                offset += depth;
                continue;
            }

            qint64 olddepth = depth;
            depth = 0;

            if(olddepth > 1) /* We have found a Good Signature! */
            {
                qint64 startoffset = (offset - olddepth) + 1;

                this->_offsetlist.append(startoffset);
                this->_signatures[startoffset] = SignatureData(offset - startoffset, SignatureDatabase::database()->signatureName(sigid));
                offset += olddepth;
                continue;
            }
        }

        offset++;
    }

    qSort(this->_offsetlist.begin(), this->_offsetlist.end());
}
