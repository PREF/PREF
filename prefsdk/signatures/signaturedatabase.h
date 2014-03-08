#ifndef PREFSDK_SIGNATUREDATABASE_H
#define PREFSDK_SIGNATUREDATABASE_H

#include <QtCore>
#include "prefsdk/sqlitewrapper/sqlite.h"
#include "patternfinder.h"

using namespace SQLite;

namespace PrefSDK
{
    class SignatureDatabase
    {
        private:
            SignatureDatabase();

        private:
            void populateDatabase();
            void updatePattern(qint64 primarykey, const QByteArray &sp, qint64 olddepth);
            qint64 insertPattern(QString name, qint64 samples, qint64 maxdepth);
            ushort makeVersion(uchar vmajor, uchar vminor);

        public:
            static void load();
            static void close();
            static SignatureDatabase* database();

        public:
            QString versionString();
            QString signatureName(qint64 id);
            qint64 signatureCount();
            qint64 signatureId(qint64 idx);
            qint64 signatureSamples(qint64 id);
            qint64 signatureMaxDepth(qint64 id);
            QByteArray signaturePattern(qint64 id);

        public:
            bool step(uchar b, qint64& depth, qint64 &sigid, QByteArray &foundsig);
            bool canContinue(uchar b, qint64 depth);
            bool isSignatureValid(const QByteArray ba, qint64 sigid);
            void updateDB(const QString& name, qint64 samples, const QByteArray &sp);
            void updateName(qint64 primarykey, const QString& newname);
            void removeSignature(qint64 id);
            uchar majorVersion();
            uchar minorVersion();

        private: /* SignatureInfo Internal Ops */
            void createVersion();
            void createCount();
            void updateCount(qint64 amount);
            qint64 querySignatureCount();
            ushort queryVersion();

        private: /* SignatureTable Internal Ops */
            void updateSamples(qint64 id, qint64 val);
            void updateMaxDepth(qint64 id, qint64 val);
            bool patternExists(const QString& name);
            qint64 querySignatureId(QString name);

        private: /* SignatureData Internal Ops */
            bool dataExists(qint64 id, qint64 depth);
            bool isDataUsed(qint64 id, qint64 depth);
            void updateSignatureData(qint64 id, const QByteArray& sp, qint64 olddepth);

        private: /* DecisionTable Internal Ops */
            bool decisionExists(uchar b, qint64 depth);
            void updateDecisionTable(qint64 id, const QByteArray& sp, qint64 olddepth);
            void removeDecisions(qint64 id, qint64 depthstart = 0, qint64 maxdepth = -1);

        private:
            static const char* DATABASE_FILE_URI;
            static const char* V_SIGNATURE_INFO_VERSION;
            static const char* V_SIGNATURE_INFO_COUNT;
            static const uchar SIGNATURE_VERSION_MAJOR;
            static const uchar SIGNATURE_VERSION_MINOR;
            static SignatureDatabase* _instance;

        private:
            SQLiteDatabase _signaturedb;
            qint64 _signaturecount;
    };
}

#endif // SIGNATUREDATABASE_H
