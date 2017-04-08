#ifndef BASICWORKER_H
#define BASICWORKER_H

#include <QThread>
#include <qhexedit/document/qhexdocument.h>

class BasicWorker : public QThread
{
    Q_OBJECT

    public:
        explicit BasicWorker(QHexDocument *document, QObject *parent = 0);

    public slots:
        void abort();

    protected:
        QHexDocument* _document;
        bool _cancontinue;
};

#endif // BASICWORKER_H
