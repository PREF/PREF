#include "signaturedatabase.h"

namespace PrefSDK
{
    const char* SignatureDatabase::DATABASE_FILE_URI = "signatures.db";
    const char* SignatureDatabase::V_SIGNATURE_INFO_VERSION = "Version";
    const char* SignatureDatabase::V_SIGNATURE_INFO_COUNT = "Count";
    const uchar SignatureDatabase::SIGNATURE_VERSION_MAJOR = 1;
    const uchar SignatureDatabase::SIGNATURE_VERSION_MINOR = 0;
    SignatureDatabase* SignatureDatabase::_instance = nullptr;

    SignatureDatabase::SignatureDatabase(): _signaturecount(-1)
    {
        QFile dbfile(QString("%1%2%3").arg(qApp->applicationDirPath(), QDir::separator(), SignatureDatabase::DATABASE_FILE_URI));
        bool dbinitialized = dbfile.exists() && (dbfile.size() > 0);

        this->_signaturedb.open(SignatureDatabase::DATABASE_FILE_URI);

        if(!dbinitialized)
            this->populateDatabase();
    }

    void SignatureDatabase::populateDatabase()
    {
        SQLiteStatement(this->_signaturedb, "CREATE TABLE IF NOT EXISTS SignatureInfo (Key TEXT NOT NULL,"
                                                                                      "Value INTEGER NOT NULL)").execute();

        SQLiteStatement(this->_signaturedb, "CREATE TABLE IF NOT EXISTS SignatureTable (SignatureID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                                       "Name TEXT UNIQUE NOT NULL,"
                                                                                       "Samples INTEGER NOT NULL,"
                                                                                       "MaxDepth INTEGER NOT NULL)").execute();

        SQLiteStatement(this->_signaturedb, "CREATE TABLE IF NOT EXISTS SignatureData (SignatureID INTEGER NOT NULL,"
                                                                                      "Depth INTEGER NOT NULL,"
                                                                                      "Byte INTEGER NOT NULL,"
                                                                                      "FOREIGN KEY(SignatureID) REFERENCES SignatureTable(SignatureID))").execute();

        SQLiteStatement(this->_signaturedb, "CREATE TABLE IF NOT EXISTS DecisionTable (Depth INTEGER NOT NULL,"
                                                                                      "Byte INTEGER NOT NULL,"
                                                                                      "SignatureID INTEGER NOT NULL)").execute();

        this->createVersion();
        this->createCount();
    }

    void SignatureDatabase::createVersion()
    {
        ushort ver = this->makeVersion(SignatureDatabase::SIGNATURE_VERSION_MAJOR, SignatureDatabase::SIGNATURE_VERSION_MINOR);

        SQLiteStatement s(this->_signaturedb, "INSERT INTO SignatureInfo (Key, Value) VALUES(?,?)");
        s.bind(1, SignatureDatabase::V_SIGNATURE_INFO_VERSION);
        s.bind(2, ver);
        s.execute();
    }

    void SignatureDatabase::createCount()
    {
        SQLiteStatement s(this->_signaturedb,"INSERT INTO SignatureInfo (Key, Value) VALUES(?,?)");
        s.bind(1, SignatureDatabase::V_SIGNATURE_INFO_COUNT);
        s.bind(2, 0);
        s.execute();
    }

    void SignatureDatabase::updateCount(qint64 amount)
    {
        SQLiteStatement s(this->_signaturedb, "UPDATE SignatureInfo SET Value = Value + :amount WHERE Key='Count'");
        s.bind(":amount", amount);
        s.execute();

        this->_signaturecount += amount;
    }

    void SignatureDatabase::updateSamples(qint64 id, qint64 val)
    {
        SQLiteStatement s(this->_signaturedb, "UPDATE SignatureTable SET Samples=? WHERE SignatureID=?");
        s.bind(1, val);
        s.bind(2, id);
        s.execute();
    }

    void SignatureDatabase::updateMaxDepth(qint64 id, qint64 val)
    {
        SQLiteStatement s(this->_signaturedb, "UPDATE SignatureTable SET MaxDepth=? WHERE SignatureID=?");
        s.bind(1, val);
        s.bind(2, id);
        s.execute();
    }

    qint64 SignatureDatabase::insertPattern(QString name, qint64 samples, qint64 maxdepth)
    {        
        SQLiteStatement s(this->_signaturedb, "INSERT INTO SignatureTable (Name, Samples, MaxDepth) VALUES (?,?,?)");
        s.bind(1, name.toUtf8().constData());
        s.bind(2, samples);
        s.bind(3, maxdepth);
        s.execute();

        return static_cast<qint64>(this->_signaturedb.lastRowId());
    }

    qint64 SignatureDatabase::querySignatureCount()
    {
        SQLiteStatement s(this->_signaturedb, "SELECT Value FROM SignatureInfo WHERE Key=?");
        s.bind(1, SignatureDatabase::V_SIGNATURE_INFO_COUNT);
        s.step();

        return static_cast<quint64>(s.result().integer64());
    }

    qint64 SignatureDatabase::querySignatureId(QString name)
    {
        SQLiteStatement s(this->_signaturedb, "SELECT SignatureID FROM SignatureTable WHERE Name=?");
        s.bind(1, name.toUtf8().constData());
        s.step();

        return static_cast<qint64>(s.column(0).integer64());
    }

    bool SignatureDatabase::dataExists(qint64 id, qint64 depth)
    {
        SQLiteStatement s(this->_signaturedb, "SELECT COUNT(*) FROM SignatureData WHERE SignatureID=? AND Depth=?");
        s.bind(1, id);
        s.bind(2, depth);

        return s.count() > 0;
    }

    bool SignatureDatabase::isDataUsed(qint64 id, qint64 depth)
    {
        SQLiteStatement s(this->_signaturedb, "SELECT COUNT(*) FROM SignatureData WHERE SignatureID<>? AND Depth=?");
        s.bind(1, id);
        s.bind(2, depth);

        return s.count() > 0;
    }

    void SignatureDatabase::updateSignatureData(qint64 id, const QByteArray &sp, qint64 olddepth)
    {
        if(olddepth > sp.length())
        {
            SQLiteStatement s(this->_signaturedb, "DELETE FROM SignatureData WHERE SignatureID=:id AND (Depth >= :depth_from AND Depth <= :depth_to)");
            s.bind(":id", id);
            s.bind(":depth_from", sp.length());
            s.bind(":depth_to", olddepth);
            s.execute();
        }

        for(qint64 i = 0; i < sp.length(); i++)
        {
            if(this->dataExists(id, i))
                continue;

            SQLiteStatement s(this->_signaturedb, "INSERT OR REPLACE INTO SignatureData (SignatureID, Depth, Byte) VALUES(?,?,?)");
            s.bind(1, id);
            s.bind(2, i);
            s.bind(3, static_cast<uchar>(sp.at(i)));
            s.execute();
        }
    }

    void SignatureDatabase::updatePattern(qint64 id, const QByteArray &sp, qint64 olddepth)
    {
        SQLiteTransaction t(this->_signaturedb);

        this->updateSignatureData(id, sp, olddepth);
        this->updateDecisionTable(id, sp, olddepth);
        t.commit();
    }

    ushort SignatureDatabase::makeVersion(uchar vmajor, uchar vminor)
    {
        return (vmajor << 8) | vminor;
    }

    ushort SignatureDatabase::queryVersion()
    {
        SQLiteStatement s(this->_signaturedb, "SELECT * FROM SignatureInfo WHERE Key = ?");
        s.bind(1, SignatureDatabase::V_SIGNATURE_INFO_VERSION);
        s.step();

        return static_cast<ushort>(s.column(1).integer());
    }

    void SignatureDatabase::load()
    {
        if(!SignatureDatabase::_instance)
            SignatureDatabase::_instance = new SignatureDatabase();
    }

    void SignatureDatabase::close()
    {
        if(SignatureDatabase::_instance)
            SignatureDatabase::_instance->close();
    }

    SignatureDatabase *SignatureDatabase::database()
    {
        return SignatureDatabase::_instance;
    }

    void SignatureDatabase::updateDB(const QString &name, qint64 samples, const QByteArray &sp)
    {
        qint64 id, olddepth;

        if(this->patternExists(name))
        {
            id = this->querySignatureId(name);
            olddepth = this->signatureMaxDepth(id);
            qint64 oldsamples = this->signatureSamples(id); /* Add Existing Samples Count */

            this->updateSamples(id, oldsamples + samples);
            this->updateMaxDepth(id, sp.length());
        }
        else
        {
            id = this->insertPattern(name, samples, sp.length());
            olddepth = sp.length();
            this->updateCount(1);
        }

        this->updatePattern(id, sp, olddepth);
    }

    void SignatureDatabase::updateName(qint64 primarykey, const QString &newname)
    {
        SQLiteStatement s(this->_signaturedb, "UPDATE SignatureTable SET Name=? WHERE SignatureID=?");
        s.bind(1, newname.toUtf8().constData());
        s.bind(2, primarykey);
        s.execute();
    }

    void SignatureDatabase::removeSignature(qint64 id)
    {
        SQLiteTransaction t(this->_signaturedb);        
        this->removeDecisions(id);

        SQLiteStatement s1(this->_signaturedb, "DELETE FROM SignatureData WHERE SignatureID=?");
        s1.bind(1, id);
        s1.execute();

        SQLiteStatement s2(this->_signaturedb, "DELETE FROM SignatureTable WHERE SignatureID=?");
        s2.bind(1, id);
        s2.execute();

        this->updateCount(-1);
        t.commit();
    }

    bool SignatureDatabase::patternExists(const QString &name)
    {
        SQLiteStatement s(this->_signaturedb, "SELECT COUNT(*) FROM SignatureTable WHERE Name=?");
        s.bind(1, name.toUtf8().constData());
        s.step();

        return s.count() > 0;
    }

    bool SignatureDatabase::decisionExists(uchar b, qint64 depth)
    {
        SQLiteStatement s(this->_signaturedb, "SELECT COUNT(*) FROM DecisionTable WHERE Byte=? AND Depth=?");
        s.bind(1, b);
        s.bind(2, depth);
        s.step();

        return s.count() > 0;
    }

    void SignatureDatabase::updateDecisionTable(qint64 id, const QByteArray &sp, qint64 olddepth)
    {
        if(olddepth > sp.length())
            this->removeDecisions(id, sp.length(), olddepth);

        for(int i = 0; i < sp.length(); i++)
        {
            uchar b = static_cast<uchar>(sp.at(i));

            if(this->decisionExists(b, i))
                continue;

            SQLiteStatement s(this->_signaturedb, "INSERT INTO DecisionTable (Depth, Byte, SignatureID) VALUES (:val_depth, :val_byte, :val_id)");
            s.bind(":val_depth", i);

            if(i < (sp.length() - 1))
                s.bind(":val_id", -1);
            else
                s.bind(":val_id", id);

            s.bind(":val_byte", b);
            s.execute();
        }
    }

    void SignatureDatabase::removeDecisions(qint64 id, qint64 depthstart, qint64 maxdepth)
    {
        if(maxdepth == -1)
            maxdepth = this->signatureMaxDepth(id);

        for(qint64 i = depthstart; i < maxdepth; i++)
        {
            if(this->isDataUsed(id, i))
                continue;

            SQLiteStatement s(this->_signaturedb, "DELETE FROM DecisionTable WHERE SignatureID=? AND Depth>=?)");
            s.bind(1, id);
            s.bind(2, i);
            s.execute();
        }
    }

    bool SignatureDatabase::canContinue(uchar b, qint64 depth)
    {
        SQLiteStatement s(SignatureDatabase::database()->_signaturedb, "SELECT COUNT(SignatureID) FROM DecisionTable WHERE Byte=? AND Depth=?");
        s.bind(1, static_cast<int>(b));
        s.bind(2, depth);
        s.step();

        return s.count() > 0;
    }

    bool SignatureDatabase::isSignatureValid(const QByteArray ba, qint64 sigid)
    {
        SQLiteStatement s1(SignatureDatabase::database()->_signaturedb, "SELECT MaxDepth FROM SignatureTable WHERE SignatureID=?");
        s1.bind(1, sigid);
        s1.step();

        qint64 maxdepth = static_cast<qint64>(s1.column(0).integer64());

        if(ba.length() != maxdepth)
            return false;

        for(qint64 i = 0; i < maxdepth; i++)
        {
            SQLiteStatement s2(SignatureDatabase::database()->_signaturedb, "SELECT Byte FROM SignatureData WHERE SignatureID=? AND Depth=?");
            s2.bind(1, sigid);
            s2.bind(2, i);
            s2.step();

            if(static_cast<uchar>(ba.at(i)) != static_cast<uchar>(s2.column(0).integer()))
                return false;
        }

        return true;
    }

    uchar SignatureDatabase::majorVersion()
    {
        return (this->queryVersion() & 0xFF00) >> 8;
    }

    uchar SignatureDatabase::minorVersion()
    {
        return this->queryVersion() & 0x00FF;
    }

    QString SignatureDatabase::versionString()
    {
        return QString("%1.%2").arg(QString::number(this->majorVersion()), QString::number(this->minorVersion()));
    }

    qint64 SignatureDatabase::signatureCount()
    {
        if(this->_signaturecount == -1)
            this->_signaturecount = this->querySignatureCount();

        return this->_signaturecount;
    }

    qint64 SignatureDatabase::signatureId(qint64 idx)
    {
        SQLiteStatement s(this->_signaturedb, "SELECT SignatureID FROM SignatureTable LIMIT 1 OFFSET ?");
        s.bind(1, idx);
        s.step();

        return static_cast<quint64>(s.result().integer64());
    }

    QString SignatureDatabase::signatureName(qint64 id)
    {
        SQLiteStatement s(this->_signaturedb, "SELECT Name FROM SignatureTable WHERE SignatureID=?");
        s.bind(1, id);
        s.step();

        return QString::fromUtf8(s.result().text());
    }

    qint64 SignatureDatabase::signatureSamples(qint64 id)
    {
        SQLiteStatement s(this->_signaturedb, "SELECT Samples FROM SignatureTable WHERE SignatureID=?");
        s.bind(1, id);
        s.step();

        return static_cast<quint64>(s.result().integer64());
    }

    qint64 SignatureDatabase::signatureMaxDepth(qint64 id)
    {
        SQLiteStatement s(this->_signaturedb, "SELECT MaxDepth FROM SignatureTable WHERE SignatureID=?");
        s.bind(1, id);
        s.step();

        return static_cast<quint64>(s.result().integer64());
    }

    QByteArray SignatureDatabase::signaturePattern(qint64 id)
    {
        qint64 maxdepth = this->signatureMaxDepth(id);

        QByteArray ba;
        ba.resize(maxdepth);

        for(int i = 0; i < maxdepth; i++)
        {
            SQLiteStatement s(this->_signaturedb, "SELECT Byte FROM SignatureData WHERE SignatureID=? AND Depth=?");
            s.bind(1, id);
            s.bind(2, i);
            s.step();

            ba[i] = static_cast<uchar>(s.result().integer());
        }

        return ba;
    }

    bool SignatureDatabase::step(uchar b, qint64& depth, qint64& sigid, QByteArray& foundsig)
    {
        sigid = -1;

        SQLiteStatement s(SignatureDatabase::database()->_signaturedb, "SELECT COUNT(SignatureID), SignatureID, Byte FROM DecisionTable WHERE Byte=? AND Depth=?");
        s.bind(1, static_cast<int>(b));
        s.bind(2, depth);
        s.step();

        if(s.count())
        {
            depth++;
            sigid = static_cast<qint64>(s.column(1).integer64());
            foundsig.append(static_cast<uchar>(s.column(2).integer()));

            if(sigid == -1)
                return true;
        }

        return false;
    }
}
