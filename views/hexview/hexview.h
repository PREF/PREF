#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "actionwidget/actiontoolbar.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "views/abstractview.h"
#include "views/hexview/optionmenu.h"
#include "datatypeswidget/datatypeswidget.h"
#include "prefsdk/sdkmanager.h"

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
        void onMapViewTriggered();
        void onHexEditCustomContextMenuRequested(const QPoint& pos);
        void onFormatParseFinished(FormatList::FormatId formatid, FormatTree* formattree);
        void disableFormatButton();

    private:
        Ui::HexView *ui;
        QHexEditData* _hexeditdata;
        ActionToolBar* _toolbar;
        QToolButton* _tbformat;
        QAction* _actbyteview;
        QAction* _actbinaryview;
        QColor _signaturecolor;
        bool _entropyenabled;
};

#endif // HEXVIEW_H
