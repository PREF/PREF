#include "visualmapmodel.h"

VisualMapModel::VisualMapModel(QObject *parent): QAbstractItemModel(parent), _mode(BinaryMapWidget::DotPlot), _startoffset(0), _currentoffset(0), _width(256)
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    this->_modenames[BinaryMapWidget::DotPlot] = "Dot Plot";
    this->_modenames[BinaryMapWidget::BytesAsPixel] = "Bytes as Pixel";
}

void VisualMapModel::setStartOffset(qint64 offset)
{
    this->_startoffset = offset;
    emit dataChanged(this->index(1, 1), this->index(1, 1));
}

void VisualMapModel::setCurrentOffset(qint64 offset)
{
    this->_currentoffset = offset;
    emit dataChanged(this->index(2, 1), this->index(2, 1));
}

int VisualMapModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant VisualMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Property";

            case 1:
                return "Value";

            default:
                break;
        }
    }


    return QVariant();
}

QVariant VisualMapModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            switch(index.row())
            {
                case 0:
                    return "Mode";

                case 1:
                    return "Start Offset";

                case 2:
                    return "Current Offset";

                case 3:
                    return "Width";

                default:
                    break;
            }
        }
        else if(index.column() == 1)
        {
            switch(index.row())
            {
                case 0:
                    return QString("'%1'").arg(this->_modenames[this->_mode]);

                case 1:
                    return QString("%1").arg(this->_startoffset, 8, 16, QLatin1Char('0')).toUpper();

                case 2:
                    return QString("%1").arg(this->_currentoffset, 8, 16, QLatin1Char('0')).toUpper();

                case 3:
                    return QString("%1").arg(this->_width, 8, 16, QLatin1Char('0')).toUpper();

                default:
                    break;
            }
        }
    }
    else if(role == Qt::EditRole)
    {
        switch(index.row())
        {
            case 0:
                return this->_mode;

            case 3:
                return this->_width;

            default:
                break;
        }
    }
    else if((role == Qt::ForegroundRole) && (index.column() == 1))
    {
        if(index.row() == 0)
            return QColor(Qt::darkGreen);

        return QColor(Qt::darkBlue);
    }
    else if((role == Qt::FontRole) && (index.column() == 1))
        return this->_monospacefont;

    return QVariant();
}

bool VisualMapModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole && index.isValid() && (index.column() == 1))
    {
        if((index.row() == 0) && (value.toInt() != this->_mode))
        {
            this->_mode = static_cast<BinaryMapWidget::DisplayMode>(value.toInt());
            emit displayModeChanged(this->_mode);
            return true;
        }
        else if((index.row() > 0) && (index.row() == 3))
        {
            this->_width = value.toLongLong();
            emit widthChanged(this->_width);
            return true;
        }

        return false;
    }

    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex VisualMapModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex VisualMapModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int VisualMapModel::rowCount(const QModelIndex &) const
{
    return 4;
}

Qt::ItemFlags VisualMapModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if((index.column() == 1) && (index.row() != 1) && (index.row() != 2))
        flags |= Qt::ItemIsEditable;

    return flags;
}
