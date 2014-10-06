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
    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(resizeSingleColumn(QModelIndex)));
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(resizeSingleColumn(QModelIndex)));
}

void FormatTreeView::setModel(FormatModel* model)
{
    QTreeView::setModel(model);
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(adaptColumns(QModelIndex,QModelIndex)));
}

void FormatTreeView::setHighlightMenuVisible(bool b)
{
    this->configureContextMenu(b);
}

void FormatTreeView::setGotoMenuVisible(bool b)
{
    this->_gotovisible = b;
}

void FormatTreeView::adaptColumns(QModelIndex topleft, QModelIndex bottomright)
{
    int firstcol = topleft.column(), lastcol = bottomright.column();

    for(int i = firstcol; i <= lastcol; i++)
        this->resizeColumnToContents(i);
}

void FormatTreeView::setSelectedFormatObjectBase(int b)
{
    QModelIndexList selidx = this->selectionModel()->selectedIndexes();
    FormatElement* formatelement = reinterpret_cast<FormatElement*>(selidx[0].internalPointer());
    formatelement->setBase(b);
}

void FormatTreeView::showContextMenu(const QPoint &pos)
{
    QItemSelectionModel* selmodel = this->selectionModel();

    if(selmodel && selmodel->selectedRows().length() == 1)
    {
        QModelIndex midx = selmodel->selectedRows()[0];
        FormatElement* formatelement = reinterpret_cast<FormatElement*>(midx.internalPointer());

        this->_structuremenu->setGotoVisible(this->_gotovisible);

        if(formatelement->elementType() == FormatElement::StructureType)
        {
            this->_structuremenu->setTitle(QString("'%1'").arg(formatelement->displayName()));
            this->_structuremenu->menuAction()->setVisible(true);
        }
        else
            this->_structuremenu->menuAction()->setVisible(false);

        if(formatelement->elementType() == FormatElement::FieldType)
            this->_copymenu->setCopyValueVisible(true);
        else
            this->_copymenu->setCopyValueVisible(false);

        this->_numericbasemenu->setBase(formatelement->base());
        this->_formatobjectmenu->exec(this->mapToGlobal(pos));
    }
}

void FormatTreeView::onTreeClicked(const QModelIndex &index)
{
    FormatElement* formatelement = reinterpret_cast<FormatElement*>(index.internalPointer());

    if(formatelement->elementType() == FormatElement::BitFieldType)
        formatelement = formatelement->parentElement(); /* Set index = BitField's Parent */

    if(formatelement->size())
        emit formatObjectSelected(formatelement);
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
    FormatElement* formatelement = this->selectedElement();
    emit gotoOffset(formatelement->offset());
}

void FormatTreeView::onStructureGotoEnd()
{
    FormatElement* formatelement = this->selectedElement();
    emit gotoOffset(formatelement->endOffset());
}

void FormatTreeView::onStructureExport()
{
    FormatElement* formatelement = this->selectedElement();
    emit exportAction(formatelement);
}

void FormatTreeView::onStructureImport()
{
    FormatElement* formatelement = this->selectedElement();
    emit importAction(formatelement);
}

void FormatTreeView::onCopyOffset()
{
    FormatElement* formatelement = this->selectedElement();
    qApp->clipboard()->setText(QString("%1").arg(formatelement->offset(), 8, 16, QLatin1Char('0')).toUpper());
}

void FormatTreeView::onCopyName()
{
    FormatElement* formatelement = this->selectedElement();
    qApp->clipboard()->setText(formatelement->displayName());
}

void FormatTreeView::onCopyValue()
{
    FormatElement* formatelement = this->selectedElement();
    qApp->clipboard()->setText(formatelement->displayValue());
}

FormatElement *FormatTreeView::selectedElement() const
{
    QItemSelectionModel* selmodel = this->selectionModel();
    QModelIndex index = selmodel->selectedIndexes()[0]; /* Get Selected Node Index*/
    return reinterpret_cast<FormatElement*>(index.internalPointer());
}

void FormatTreeView::configureContextMenu(bool highlightvisible)
{
    if(!this->_formatobjectmenu)
    {
        this->setContextMenuPolicy(Qt::CustomContextMenu);

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
    FormatElement* formatelement = this->selectedElement();

    if(formatelement->elementType() == FormatElement::BitFieldType)
        formatelement = formatelement->parentElement(); /* Change Object to BitField's parent */

    if(formatelement->size())
    {
        if(set)
            emit setBackColor(formatelement);
        else
            emit removeBackColor(formatelement);
    }
}

void FormatTreeView::resizeSingleColumn(const QModelIndex& index)
{
    this->adaptColumns(this->model()->index(index.row(), 0), this->model()->index(index.row(), this->model()->columnCount() - 1));
}
