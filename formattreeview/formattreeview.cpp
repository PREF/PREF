#include "formattreeview.h"

FormatTreeView::FormatTreeView(QWidget *parent): QTreeView(parent)
{
    this->_formatobjectmenu = nullptr;
    this->_highlightmenu = nullptr;
    this->_numericbasemenu = nullptr;

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
    const ElementHeader* elemhdr = reinterpret_cast<const ElementHeader*>(selidx[0].internalPointer());

    elemhdr->SetBase(b);
    this->resizeTreeColumns();
}

void FormatTreeView::showContextMenu(const QPoint &pos)
{
    QItemSelectionModel* selmodel = this->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex midx = selmodel->selectedRows()[0];
        const ElementHeader* elemhdr = reinterpret_cast<const ElementHeader*>(midx.internalPointer());

        this->_structuremenu->setGotoVisible(this->_gotovisible);

        if(elemhdr->ElementType == ElementType::Structure)
        {
            this->_structuremenu->setTitle(QString("'%1'").arg(QString::fromUtf8(elemhdr->DisplayName())));
            this->_structuremenu->menuAction()->setVisible(true);
        }
        else
            this->_structuremenu->menuAction()->setVisible(false);

        if(elemhdr->ElementType == ElementType::Field)
            this->_copymenu->setCopyValueVisible(true);
        else
            this->_copymenu->setCopyValueVisible(false);

        this->_numericbasemenu->setBase(elemhdr->Base());
        this->_formatobjectmenu->exec(this->mapToGlobal(pos));
    }
}

void FormatTreeView::onTreeClicked(const QModelIndex &index)
{
    const ElementHeader* elemhdr = reinterpret_cast<const ElementHeader*>(index.internalPointer());

    if(elemhdr->ElementType == ElementType::BitField)
        elemhdr = elemhdr->ParentElement(); /* Set index = BitField's Parent */

    if(elemhdr->Size())
        emit formatObjectSelected(elemhdr);
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
    const ElementHeader* elemhdr = this->selectedElement();
    emit gotoOffset(elemhdr->Offset());
}

void FormatTreeView::onStructureGotoEnd()
{
    const ElementHeader* elemhdr = this->selectedElement();
    emit gotoOffset(elemhdr->EndOffset());
}

void FormatTreeView::onStructureExport()
{
    const ElementHeader* elemhdr = this->selectedElement();
    emit exportAction(elemhdr);
}

void FormatTreeView::onStructureImport()
{
    const ElementHeader* elemhdr = this->selectedElement();
    emit importAction(elemhdr);
}

void FormatTreeView::onCopyOffset()
{
    const ElementHeader* elemhdr = this->selectedElement();
    qApp->clipboard()->setText(QString("%1").arg(elemhdr->Offset(), 8, 16, QLatin1Char('0')).toUpper());
}

void FormatTreeView::onCopyName()
{
    const ElementHeader* elemhdr = this->selectedElement();
    qApp->clipboard()->setText(elemhdr->DisplayName());
}

void FormatTreeView::onCopyValue()
{
    const ElementHeader* elemhdr = this->selectedElement();
    qApp->clipboard()->setText(elemhdr->DisplayValue());
}

const ElementHeader *FormatTreeView::selectedElement() const
{
    QItemSelectionModel* selmodel = this->selectionModel();
    QModelIndex index = selmodel->selectedIndexes()[0]; /* Get Selected Node Index*/
    return reinterpret_cast<const ElementHeader*>(index.internalPointer());
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
    const ElementHeader* elementhdr = this->selectedElement();

    if(elementhdr->ElementType == ElementType::BitField)
        elementhdr = elementhdr->ParentElement(); /* Change Object to BitField's parent */

    if(elementhdr->Size())
    {
        if(set)
            emit setBackColor(elementhdr);
        else
            emit removeBackColor(elementhdr);
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
