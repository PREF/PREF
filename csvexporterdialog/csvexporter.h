#ifndef CSVEXPORTER_H
#define CSVEXPORTER_H

#include "csvexporterdialog.h"
#include "csvexporterworker.h"

class CSVExporter: public QObject
{
    Q_OBJECT

    public:
        CSVExporter(QObject* parent = 0);
        void dump(const QString& file, QAbstractItemModel* model);
};

#endif // CSVEXPORTER_H
