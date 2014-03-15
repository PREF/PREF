#include "formattreeview.h"

FormatTreeView::FormatTreeView(QWidget *parent): QTreeView(parent)
{
    this->_formatobjectmenu = nullptr;
    this->_highlightmenu = nullptr;
    this->_numericbasemenu = nullptr;

    //this->setModel(FormatModel::empty()); /* An empty model in order to display columns */
    this->configureContextMenu(true);

    /* FormatTreeView Signal/Slots */
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(onTreeClicked(QModelIndex)));
    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(resizeColumnsOnExpandedOrCollapsed(QModelIndex)));
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(resizeColumnsOnExpandedOrCollapsed(QModelIndex)));
}

void FormatTreeView::setModel(FormatModel* model)
{
    QTreeView::setModel(model);
    this->resizeTreeColumns();
}

void FormatTreeView::setHighlightMenuVisible(bool b)
{
    this->configureContextMenu(b);
}

void FormatTreeView::setGotoMenuVisible(bool b)
{
    this->_gotovisible = b;
}

void FormatTreeView::setSelectedFormatObjectBase(int b)
{
    QModelIndexList selidx = this->selectionModel()->selectedIndexes();
    FormatObject* formatobj = reinterpret_cast<FormatObject*>(selidx[0].internalPointer());

    formatobj->setBase(b);
    this->resizeTreeColumns();
}

void FormatTreeView::showContextMenu(const QPoint &pos)
{
    QItemSelectionModel* selmodel = this->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex midx = selmodel->selectedRows()[0];
        FormatObject* formatobj = (FormatObject*)midx.internalPointer();

        this->_structuremenu->setGotoVisible(this->_gotovisible);

        if(formatobj->objectType() == FormatObject::StructureType)
        {
            this->_structuremenu->setTitle(QString("'%1'").arg(formatobj->displayName()));
            this->_structuremenu->menuAction()->setVisible(true);
        }
        else
            this->_structuremenu->menuAction()->setVisible(false);

        if(formatobj->objectType() == FormatObject::FieldType)
            this->_copymenu->setCopyValueVisible(true);
        else
            this->_copymenu->setCopyValueVisible(false);

        this->_numericbasemenu->setBase(formatobj->base());
        this->_formatobjectmenu->exec(this->mapToGlobal(pos));
    }
}

void FormatTreeView::onTreeClicked(const QModelIndex &index)
{
    FormatObject* formatobj = reinterpret_cast<FormatObject*>(index.internalPointer());

    if(formatobj->objectType() == FormatObject::BitFieldType)
        formatobj = formatobj->parentObject(); /* Set index = BitField's Parent */

    if(formatobj->size())
        emit formatObjectSelected(formatobj);
}

void FormatTreeView::setBackColor()
{
    this->updateColor(true);
}

void FormatTreeView::removeBackColor()
{
    this->updateColor(false);
}

void FormatTreeView::onStructureGotoStart()
{
    FormatObject* formatobj = this->selectedFormatObject();
    emit gotoOffset(formatobj->offset());
}

void FormatTreeView::onStructureGotoEnd()
{
    FormatObject* formatobj = this->selectedFormatObject();
    emit gotoOffset(formatobj->endOffset());
}

void FormatTreeView::onStructureExport()
{
    FormatObject* formatobj = this->selectedFormatObject();
    emit exportAction(formatobj);
}

void FormatTreeView::onStructureImport()
{
    FormatObject* formatobj = this->selectedFormatObject();
    emit importAction(formatobj);
}

void FormatTreeView::onCopyOffset()
{
    FormatObject* formatobj = this->selectedFormatObject();
    qApp->clipboard()->setText(QString("%1").arg(formatobj->offset(), 8, 16, QLatin1Char('0')).toUpper());
}

void FormatTreeView::onCopyName()
{
    FormatObject* formatobj = this->selectedFormatObject();
    qApp->clipboard()->setText(formatobj->displayName());
}

void FormatTreeView::onCopyValue()
{
    FormatObject* formatobj = this->selectedFormatObject();
    qApp->clipboard()->setText(formatobj->displayValue());
}

FormatObject *FormatTreeView::selectedFormatObject() const
{
    QItemSelectionModel* selmodel = this->selectionModel();
    QModelIndex index = selmodel->selectedIndexes()[0]; /* Get Selected Node Index*/
    return reinterpret_cast<FormatObject*>(index.internalPointer());
}

void FormatTreeView::configureContextMenu(bool highlightvisible)
{
    if(!this->_formatobjectmenu)
    {
        this->_formatobjectmenu = new QMenu(this);
        this->_highlightmenu = new QMenu("Highlight");
        this->_copymenu = new CopyMenu(this);
        this->_numericbasemenu = new NumericBaseMenu(this);
        this->_structuremenu = new StructureMenu(this);

        QAction* actsetbackcolor = this->_highlightmenu->addAction("Set Back Color");
        QAction* actremovebackcolor = this->_highlightmenu->addAction("Remove Back Color");

        connect(actsetbackcolor, SIGNAL(triggered()), this, SLOT(setBackColor()));
        connect(actremovebackcolor, SIGNAL(triggered()), this, SLOT(removeBackColor()));
        connect(this->_copymenu, SIGNAL(copyOffset()), this, SLOT(onCopyOffset()));
        connect(this->_copymenu, SIGNAL(copyName()), this, SLOT(onCopyName()));
        connect(this->_copymenu, SIGNAL(copyValue()), this, SLOT(onCopyValue()));
        connect(this->_numericbasemenu, SIGNAL(baseChanged(int)), this, SLOT(setSelectedFormatObjectBase(int)));
        connect(this->_structuremenu, SIGNAL(gotoStartAction()), this, SLOT(onStructureGotoStart()));
        connect(this->_structuremenu, SIGNAL(gotoEndAction()), this, SLOT(onStructureGotoEnd()));
        connect(this->_structuremenu, SIGNAL(exportAction()), this, SLOT(onStructureExport()));
        connect(this->_structuremenu, SIGNAL(importAction()), this, SLOT(onStructureImport()));
    }

    this->_formatobjectmenu->addMenu(this->_copymenu);
    this->_formatobjectmenu->addSeparator();

    if(highlightvisible)
        this->_formatobjectmenu->addMenu(this->_highlightmenu);

    this->_formatobjectmenu->addMenu(this->_numericbasemenu);
    this->_formatobjectmenu->addMenu(this->_structuremenu);
}

void FormatTreeView::updateColor(bool set)
{
    FormatObject* formatobj = this->selectedFormatObject();

    if(formatobj->objectType() == FormatObject::BitFieldType)
        formatobj = formatobj->parentObject(); /* Change Object to BitField's parent */

    if(formatobj->size())
    {
        if(set)
            emit setBackColor(formatobj);
        else
            emit removeBackColor(formatobj);
    }
}

void FormatTreeView::resizeTreeColumns()
{
    for(int i = 0; i < this->model()->columnCount(); i++)
        this->resizeColumnToContents(i);
}

void FormatTreeView::resizeColumnsOnExpandedOrCollapsed(const QModelIndex &)
{
    this->resizeTreeColumns();
}
