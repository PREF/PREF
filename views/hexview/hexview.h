#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "actionwidget/actiontoolbar.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "views/abstractview.h"
#include "views/hexview/optionmenu.h"
#include "datatypesview/datatypesview.h"
#include "prefsdk/sdkmanager.h"
#include "binaryviewdialog/binaryviewdialog.h"
#include "disassemblerdialog/disassemblerdialog.h"
#include "formatsdialog.h"

using namespace PrefSDK;

namespace Ui {
    class HexView;
}

class HexView : public AbstractView
{
    Q_OBJECT
    
    public:
        explicit HexView(QHexEditData* hexeditdata, const QString &viewname, QLabel* labelinfo, QWidget *parent = 0);
        void save(QString filename);
        void save();
        ~HexView();

    public: /* Overriden Methods */
        virtual bool canSave() const;
        virtual void updateStatusBar();

    private:
        void createToolBar();
        void inspectData(QHexEditData* hexeditdata);

    private slots:
        void updateOffset(qint64);
        void updateSelLength(qint64 size);
        void onWorkStarted();
        void onWorkFinished();
        void onLoadFormatClicked();
        void onMapViewTriggered();
        void onBinaryViewTriggered();
        void onDisassemblerTriggered();
        void onHexEditCustomContextMenuRequested(const QPoint& pos);

    private:
        Ui::HexView *ui;
        BinaryViewDialog* _binaryviewdialog;
        DisassemblerDialog* _disassemblerdialog;
        QHexEditData* _hexeditdata;
        ActionToolBar* _toolbar;
        QToolButton* _tbformat;
        QAction* _actbyteview;
        QAction* _actbinaryview;
        QAction* _actdisassembler;
        QColor _signaturecolor;
        bool _entropyenabled;
};

#endif // HEXVIEW_H
