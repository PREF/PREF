#ifndef SIGNATUREDATABASEDIALOG_H
#define SIGNATUREDATABASEDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/signaturedatabasemodel/signaturedatabasemodel.h"
#include "viewmodels/filefiltermodel/filefiltermodel.h"
#include "prefsdk/signatures/signaturedatabase.h"
#include "prefsdk/signatures/patternfinder.h"
#include "qhexedit/qhexedit.h"
#include "elaboratetoolbar.h"

using namespace PrefSDK;

namespace Ui {
class SignatureDatabaseDialog;
}

class SignatureDatabaseDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit SignatureDatabaseDialog(QWidget *parent = 0);
        ~SignatureDatabaseDialog();

    private slots:
        void on_tbFolder_clicked();
        void on_tabWidget_currentChanged(int index);
        void on_leFolder_textChanged(const QString &text);
        void on_leSignatureName_textEdited(const QString &text);
        void on_tvSignatures_clicked(const QModelIndex &index);
        void saveSignature();
        void deleteSignature();
        void markSignature();
        void analyzeFiles();

    private:
        void createToolbar();
        void displayFiles();
        void displayDatabaseInfo();
        void resizeColumns(QAbstractItemModel *model, QTreeView *view);

    private:
        Ui::SignatureDatabaseDialog *ui;
        SignatureDatabaseModel* _signaturedbmodel;
        FileFilterModel* _filefiltermodel;
        ElaborateToolBar* _toolbar;
        QAction* _actsavesignature;
        QAction* _actdeletesignature;
        QAction* _actmarksignature;
        QByteArray _startpattern;
        int _analyzedsamples;
};

#endif // SIGNATUREDATABASEDIALOG_H
