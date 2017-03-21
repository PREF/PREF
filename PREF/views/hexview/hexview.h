#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "actionwidget/actiontoolbar.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "datatypeswidget/datatypeswidget.h"
#include "views/abstractview.h"

namespace Ui {
    class HexView;
}

class HexView : public AbstractView
{
    Q_OBJECT
    
    public:
        explicit HexView(QHexEditData* hexeditdata, const QString &loadedfile, QLabel* labelinfo, QWidget *parent = 0);
        virtual ~HexView();
        virtual void saveAs();
        virtual void save();

    public: /* Overriden Methods */
        virtual bool canSave() const;
        virtual bool canSaveAs() const;
        virtual void updateStatusBar();
        virtual LogWidget* logWidget();

    private:
        void createToolBar();
        void hideFormatView();
        void inspectData(QHexEditData* hexeditdata);
        void selectPage(QWidget* page);

    private slots:
        void updateOffset(qint64);
        void updateSelLength(qint64 size);
        void onWorkStarted();
        void onWorkFinished();
        void onMapViewTriggered();
        void onHexEditCustomContextMenuRequested(const QPoint& pos);
        void onFormatParsingStarted();
        void onFormatParsingFinished();
        void onFormatParsingFailed();

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
