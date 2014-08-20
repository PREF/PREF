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
        explicit HexView(QHexEditData* hexeditdata, const QString &viewname, QLabel* labelinfo, AbstractView::LoadedViews& loadedviews, QWidget *parent = 0);
        virtual ~HexView();
        void save(QString filename);
        void save();

    public: /* Overriden Methods */
        virtual QHexEditData* data();
        virtual bool canSave() const;
        virtual void updateStatusBar();
        virtual void log(const QString& text);
        virtual void logLine(const QString& text, LogWidget::LogLevel loglevel = LogWidget::Nothing);

    private:
        void createToolBar();
        void inspectData(QHexEditData* hexeditdata);
        void selectPage(QWidget* page);

    private slots:
        void updateOffset(qint64);
        void updateSelLength(qint64 size);
        void onWorkStarted();
        void onWorkFinished();
        void onMapViewTriggered();
        void onHexEditCustomContextMenuRequested(const QPoint& pos);
        void onFormatParseStarted();
        void onFormatParseFinished(FormatTree* formattree);
        void disableFormatButton();

    private:
        Ui::HexView *ui;
        ActionToolBar* _toolbar;
        QToolButton* _tbformat;
        QAction* _actbyteview;
        QAction* _actbinaryview;
        QColor _signaturecolor;
        bool _entropyenabled;
};

#endif // HEXVIEW_H
