#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexedit.h"
#include "viewmodels/exportmodel/exportmodel.h"

using namespace PrefSDK;

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT
    
    public:
        explicit ExportDialog(QHexEdit* hexedit, ByteBuffer *bytebuffer, QWidget *parent = 0);
        void setFixedRange(qint64 start, qint64 end);
        ~ExportDialog();

    private slots:
        void on_rbRange_toggled(bool checked);
        void on_tbBrowse_clicked();
        void on_tvExporters_clicked(const QModelIndex &index);
        void on_pbExport_clicked();

    private:
        void validateFields();
        bool queryRange(lua_Integer& from, lua_Integer& to);

    private:
        Ui::ExportDialog *ui;
        ExportModel* _exportmodel;
        ExportDefinition::Ptr _selexporter;
        lua_State* _state;
        QHexEdit* _hexedit;
        ByteBuffer* _bytebuffer;
        QString _filename;
};

#endif // EXPORTDIALOG_H
