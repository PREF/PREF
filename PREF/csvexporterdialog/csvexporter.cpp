#include "csvexporter.h"

CSVExporter::CSVExporter(QObject *parent): QObject(parent)
{
}

void CSVExporter::dump(const QString &file, QAbstractItemModel *model)
{
    CSVExporterDialog ed(file, model);
    ed.exec();
}
