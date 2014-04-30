#ifndef FILEFORMATVIEW_H
#define FILEFORMATVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "views/abstractview.h"
#include "datatypesview/datatypesview.h"
#include "hexeditviewpage/hexeditviewpage.h"
#include "binaryviewpage/binaryviewpage.h"
#include "chartviewpage/chartviewpage.h"
#include "disassemblerviewpage/disassemblerviewpage.h"
#include "stringfinderviewpage/stringfinderviewpage.h"
#include "prefsdk/sdkmanager.h"

using namespace PrefSDK;

namespace Ui {
class FormatView;
}

class FormatView : public AbstractView
{
    Q_OBJECT
    
    public:
        enum Views {HexView, BinaryView, ChartView, DisassemblerView, StringFinderView, CustomView};
        explicit FormatView(QHexEditData* hexeditdata, QWidget *parent = 0);
        void save();
        void save(QString filename);
        FormatView::Views currentView();
        ~FormatView();

    public: /* Overriden Methods */
        virtual bool canSave() const;

    protected:
        virtual void closeEvent(QCloseEvent* event);

    signals:
        void viewChanged(FormatView::Views v);

    private slots:
        void jumpToOffset(qint64 offset, qint64 length = 1);
        void on_tabWidget_currentChanged(int);
        void onFormatLoaded(FormatList::FormatId formatid);

    private:
        void createHexView();
        void createBinaryView();
        void createChartView();
        void createStringFinderView();
        void createDisassemblerView(FormatTree *formattree);

    private:
        Ui::FormatView *ui;
        FormatList::FormatId _formatid;
        HexEditViewPage* _hexeditview;
        BinaryViewPage* _binaryview;
        ChartViewPage* _chartview;
        DisassemblerViewPage* _disassemblerview;
        StringFinderViewPage* _stringfinderview;
        QHexEditData* _hexeditdata;
};

#endif // FILEFORMATVIEW_H
