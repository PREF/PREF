#include "crossreferencemodel.h"

CrossReferenceModel::CrossReferenceModel(ReferenceSet* referenceset, const QList<Reference *> references, DisassemblerListing *listing, QObject *parent): QAbstractItemModel(parent), _address(referenceset->startAddress()), _listing(listing), _references(references)
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);
}

CrossReferenceModel::CrossReferenceModel(Block *block, DisassemblerListing *listing, QObject *parent): QAbstractItemModel(parent), _address(block->startAddress()), _listing(listing)
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    ReferenceTable* referenceset = listing->referenceTable();
    this->_references = referenceset->references(block)->referenceList();
}

CrossReferenceModel::CrossReferenceModel(const DataValue &address, DisassemblerListing *listing, QObject *parent): QAbstractItemModel(parent), _address(address), _listing(listing)
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    ReferenceTable* referenceset = listing->referenceTable();
    this->_references = referenceset->references(address)->referenceList();
}

int CrossReferenceModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant CrossReferenceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Direction";

            case 1:
                return "Virtual Address";

            case 2:
                return "Instruction";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant CrossReferenceModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        Reference* r = reinterpret_cast<Reference*>(index.internalPointer());

        if(index.column() == 0)
        {
            if(r->referencedBy() > this->_address)
                return "Down";
            else if(r->referencedBy() < this->_address)
                return "Up";
            else
                return "---";
        }
        else if(index.column() == 1)
            return r->referencedBy().toString(16).append("h");
        //else if(index.column() == 2)
            //return DisassembledInstructionManager::generateInstruction(item.second);
    }
    else if(role == Qt::FontRole && index.column() > 0)
        return this->_monospacefont;
    else if(role == Qt::ForegroundRole && index.column() == 1)
        return QColor(Qt::darkBlue);

    return QVariant();
}

QModelIndex CrossReferenceModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column, this->_references[row]);
}

QModelIndex CrossReferenceModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int CrossReferenceModel::rowCount(const QModelIndex&) const
{
    return this->_references.length();
}

Qt::ItemFlags CrossReferenceModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
