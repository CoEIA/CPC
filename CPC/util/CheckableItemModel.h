#ifndef CHECKABLEITEMMODEL_H
#define CHECKABLEITEMMODEL_H

#include <QStandardItemModel>

class CheckableItemModel : public QStandardItemModel
{
public:
    CheckableItemModel(int r, int c): QStandardItemModel(r, c) {}

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole)
    {
        QStandardItem *item = itemFromIndex(index);
        for( int i = 0; i < item->rowCount() ; i++ )
        {
            QStandardItem *childItem = item->child(i);
            setData(childItem->index(), value, role);
        }
        return QStandardItemModel::setData(index, value, role);
    }
};

#endif // CHECKABLEITEMMODEL_H
