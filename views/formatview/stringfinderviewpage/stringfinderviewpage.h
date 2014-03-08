#ifndef STRINGFINDERVIEWPAGE_H
#define STRINGFINDERVIEWPAGE_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#include "viewmodels/stringfindermodel/stringfindermodel.h"
#include "elaboratetoolbar.h"
#include "prefsdk/io/bytebuffer.h"
#include "stringfinderhelper.h"

using namespace PrefSDK;

namespace Ui {
class StringFinderViewPage;
}

class StringFinderViewPage : public QWidget
{
    Q_OBJECT
    
    public:
        explicit StringFinderViewPage(ByteBuffer* bytebuffer, QWidget *parent = 0);
        ~StringFinderViewPage();
    
    private:
        void createToolBar();
        void checkValidState();
        void createStringListActions();

    private slots:
        void on_sbMinLength_valueChanged(int);
        void on_sbMaxLength_valueChanged(int);
        void on_stringList_customContextMenuRequested(const QPoint &pos);
        void on_stringList_doubleClicked(const QModelIndex &index);
        void onStringFinderFinished();
        void onStartTriggered();
        void onGotoTriggered();
        void onExportCsvTriggered();
        void onCopyAddressTriggered();
        void onCopyStringTriggered();
        void onFilterChanged();

    signals:
        void gotoTriggered(qint64, qint64);

    private:
        Ui::StringFinderViewPage *ui;
        ByteBuffer* _bytebuffer;
        StringFinderModel* _stringfindermodel;
        StringFinderHelper* _stringfinderhelper;
        ElaborateToolBar* _toolbar;
        QMenu* _slmenu;
        QAction* _actgoto;
        QAction* _actcopyaddress;
        QAction* _actcopystring;
        QAction* _actexportcsv;
};

#endif // STRINGFINDERVIEWPAGE_H
