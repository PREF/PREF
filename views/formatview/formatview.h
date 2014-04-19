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
#include "formatoptionsdialog.h"
#include "formatsdialog.h"

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

    signals:
        void viewChanged(FormatView::Views v);

    private slots:
        void on_tabWidget_currentChanged(int);
        void jumpToOffset(qint64 offset, qint64 length = 1);
        void on_tbFormats_clicked();
        void on_tbFormatOptions_clicked();
        void on_tbSignatureScanner_clicked();
        void on_tbEntropy_clicked();

    private:
        void createHexView();
        void createBinaryView();
        void createChartView();
        void createDisassemblerView();
        void createStringFinderView();

    private:
        Ui::FormatView *ui;
        HexEditViewPage* _hexeditview;
        BinaryViewPage* _binaryview;
        ChartViewPage* _chartview;
        DisassemblerViewPage* _disassemblerview;
        StringFinderViewPage* _stringfinderview;
        FormatList::Format _format;
        QHexEditData* _hexeditdata;
};

#endif // FILEFORMATVIEW_H
