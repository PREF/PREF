#ifndef STRINGSWORKER_H
#define STRINGSWORKER_H

#include "basicworker.h"
#include <support/byteelaborator.h>

using namespace PrefLib::Support;

class StringsWorker : public BasicWorker
{
    Q_OBJECT

    public:
        explicit StringsWorker(ByteElaborator::StringList& stringlist, QHexDocument *document, QObject *parent = 0);

    protected:
        virtual void run();

    signals:
        void searchEnable(bool b);

    private:
        ByteElaborator::StringList& _stringlist;
};

#endif // STRINGSWORKER_H
