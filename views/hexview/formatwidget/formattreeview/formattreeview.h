#ifndef FORMATTREEVIEW_H
#define FORMATTREEVIEW_H

#include "copymenu.h"
#include "numericbasemenu.h"
#include "structuremenu.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "qhexedit/qhexeditdata.h"
#include "exportdialog.h"
#include "prefsdk/type/datavalue.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

using namespace PrefSDK;

class FormatTreeView : public QTreeView
{
    Q_OBJECT

    public:
        explicit FormatTreeView(QWidget *parent = 0);
        virtual void setModel(FormatModel *model);
        void setHighlightMenuVisible(bool b);
        void setGotoMenuVisible(bool b);

    private slots:
        void adaptColumns(QModelIndex topleft, QModelIndex bottomright);
        void resizeSingleColumn(const QModelIndex& index);
        void setSelectedFormatObjectBase(int b);
        void showContextMenu(const QPoint& pos);
        void onTreeClicked(const QModelIndex &index);
        void setBackColor();
        void removeBackColor();
        void onGotoOffset();
        void onStructureGotoStart();
        void onStructureGotoEnd();
        void onStructureExport();
        void onStructureImport();
        void onCopyOffset();
        void onCopyName();
        void onCopyValue();

    private:
        FormatElement* selectedElement() const;
        void configureContextMenu(bool highlightvisible);
        void updateColor(bool set);

    signals:
        void setBackColor(FormatElement*);
        void removeBackColor(FormatElement*);
        void formatObjectSelected(FormatElement*);
        void exportAction(FormatElement*);
        void importAction(FormatElement*);
        void gotoOffset(qint64);

    private:
        QMenu* _formatobjectmenu;
        QMenu* _highlightmenu;
        QAction* _actiongoto;
        CopyMenu* _copymenu;
        NumericBaseMenu* _numericbasemenu;
        StructureMenu* _structuremenu;
        bool _gotovisible;
};

#endif // FORMATTREEVIEW_H
