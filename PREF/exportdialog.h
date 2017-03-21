#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <prefcontext.h>
#include "qhexedit/qhexedit.h"
#include "viewmodels/exportmodel/exportermodel.h"

using namespace PrefLib;
using namespace PrefLib::Exporter;

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT
    
    public:
        explicit ExportDialog(QHexEdit* hexedit, QWidget *parent = 0);
        ~ExportDialog();
        void setFixedRange(qint64 start, qint64 end);
        ExporterDefinition* selectedExporter() const;
        const QString& fileName() const;
        quint64 startOffset() const;
        quint64 endOffset() const;

    private slots:
        void on_rbRange_toggled(bool checked);
        void on_tbBrowse_clicked();
        void on_tvExporters_clicked(const QModelIndex &index);
        void on_pbExport_clicked();

    private:
        void validateFields();
        bool queryRange();

    private:
        Ui::ExportDialog *ui;
        ExporterModel* _exportmodel;
        ExporterDefinition* _selexporter;
        QHexEdit* _hexedit;
        quint64 _startoffset;
        quint64 _endoffset;
        QString _filename;
};

#endif // EXPORTDIALOG_H
