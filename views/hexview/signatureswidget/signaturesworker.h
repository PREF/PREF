#ifndef SIGNATURESWORKER_H
#define SIGNATURESWORKER_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "prefsdk/signatures/signaturedatabase.h"
#include "views/hexview/worker.h"

using namespace PrefSDK;

class SignaturesWorker : public Worker
{
    Q_OBJECT

    private: /* Secondary Data Structures */
        typedef QHash<uchar, qint64> SignatureIdCache;
        typedef QSet<uchar> InvalidByteSetCache;

    private: /* Primary Data Structures */
        typedef QHash<qint64, QByteArray> SignaturePatternCache;
        typedef QHash<qint64, QString> SignatureNameCache;
        typedef QHash<qint64, SignatureIdCache> DecisionTableCache;
        typedef QHash<qint64, InvalidByteSetCache> InvalidPathCache;

    public:
        typedef QPair<qint64, QString> SignatureData;
        typedef QList<qint64> OffsetList;
        typedef QHash<qint64, SignatureData> SignatureMap;

    public:
        explicit SignaturesWorker(QObject *parent = 0);
        void setData(QHexEditData* hexeditdata);
        const SignaturesWorker::OffsetList& offsetList() const;
        const SignaturesWorker::SignatureMap& signatures() const;

    private: /* DB Cache in order to increase scanning performance */
        bool step(qint64 depth, uchar b, qint64 &sigid);
        QByteArray signaturePattern(qint64 sigid);
        QString signatureName(qint64 sigid);
        bool isSignatureValid(const QByteArray& ba, qint64 sigid);

    protected:
        virtual void run();

    private:
        QHexEditData* _hexeditdata;
        OffsetList _offsetlist;
        SignatureMap _signatures;
        SignatureNameCache _signamecache;
        SignaturePatternCache _sigpatterncache;
        InvalidPathCache _invalidpathcache;
        DecisionTableCache _dectablecache;
};

#endif // SIGNATURESWORKER_H
