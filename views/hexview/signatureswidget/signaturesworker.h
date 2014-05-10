#ifndef SIGNATURESWORKER_H
#define SIGNATURESWORKER_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "prefsdk/signatures/signaturedatabase.h"

using namespace PrefSDK;

class SignaturesWorker : public QThread
{
    Q_OBJECT

    public:
        typedef QPair<qint64, QString> SignatureData;
        typedef QList<qint64> OffsetList;
        typedef QHash<qint64, SignatureData> SignatureMap;

    public:
        explicit SignaturesWorker(QObject *parent = 0);
        void setData(QHexEditData* hexeditdata);
        const SignaturesWorker::OffsetList& offsetList() const;
        const SignaturesWorker::SignatureMap& signatures() const;

    protected:
        virtual void run();

    private:
        QHexEditData* _hexeditdata;
        OffsetList _offsetlist;
        SignatureMap _signatures;

};

#endif // SIGNATURESWORKER_H
