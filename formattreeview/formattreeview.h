#ifndef FORMATTREEVIEW_H
#define FORMATTREEVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "copymenu.h"
#include "numericbasemenu.h"
#include "structuremenu.h"
#include "viewmodels/formatmodel/formatmodel.h"
#include "qhexedit/qhexeditdata.h"
#include "exportdialog.h"

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
        void resizeColumnsOnExpandedOrCollapsed(const QModelIndex&);
        void setSelectedFormatObjectBase(int b);
        void showContextMenu(const QPoint& pos);
        void onTreeClicked(const QModelIndex &index);
        void setBackColor();
        void removeBackColor();
        void onStructureGotoStart();
        void onStructureGotoEnd();
        void onStructureExport();
        void onCopyOffset();
        void onCopyName();
        void onCopyValue();

    private:
        FormatObject* selectedFormatObject() const;
        void configureContextMenu(bool highlightvisible);
        void updateColor(bool set);
        void resizeTreeColumns();

    signals:
        void setBackColor(FormatObject*);
        void removeBackColor(FormatObject*);
        void formatObjectSelected(FormatObject*);
        void exportAction(FormatObject*);
        void gotoOffset(qint64);

    private:
        QMenu* _formatobjectmenu;
        QMenu* _highlightmenu;
        CopyMenu* _copymenu;
        NumericBaseMenu* _numericbasemenu;
        StructureMenu* _structuremenu;
        bool _gotovisible;
};

#endif // FORMATTREEVIEW_H
