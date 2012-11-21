#ifndef MYQTREEVIEW_H
#define MYQTREEVIEW_H

#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>
#include <QStandardItem>

class MyQTreeView: public QTreeView {
public:
    MyQTreeView(QWidget* parent=0): QTreeView(parent){}
    virtual ~MyQTreeView() {}

protected:
    void resettingCheckBox (QModelIndex& parentIndex) {
        if ( ! parentIndex.isValid() )
            return;

        QString text = parentIndex.data( Qt::DisplayRole ).value<QString>();
        qDebug() << "parent is: " << text;

        if ( model()->hasChildren(parentIndex) ) {
            for( int i = 0; i < model()->rowCount(parentIndex) ; i++ ) {
                QModelIndex childIndex = model()->index( i, 0, parentIndex );

                if ( model()->hasChildren(childIndex) ) {
                    qDebug() << text << " have child " ;
                    resettingCheckBox(childIndex);
                }
                else {
                    QString text = childIndex.data( Qt::DisplayRole ).value<QString>();
                    qDebug() << "child is: " << text;

                    QStandardItem *parentItem = static_cast<QStandardItem*> (parentIndex.internalPointer());
                    QStandardItem *childItem  = static_cast<QStandardItem*> (childIndex.internalPointer());

                    if ( parentItem->checkState() == Qt::Checked ) {
                        childItem->setCheckState( Qt::Unchecked);
                        childItem->setCheckable(true);
                        qDebug() << "after child item " << childItem->checkState();

                    }
                    else {
                        childItem->setCheckState(  Qt::Checked );
                        childItem->setCheckable(false);
                        qDebug() << "after child item " << childItem->checkState();
                    }

                    this->repaint();
                }
            }
        }
    }

    void mousePressEvent (QMouseEvent *event) {
        QModelIndex index = indexAt(event->pos());

        if(index.isValid()) {
            QStyleOptionButton opt;
            opt.QStyleOption::operator=(viewOptions());
            opt.rect = visualRect(index);
            QRect rect = style()->subElementRect(QStyle::SE_ViewItemCheckIndicator, &opt);

            if (rect.contains(event->pos())) {
                //QTreeView::mousePressEvent(event);
                resettingCheckBox(index);
            }
            else
                QTreeView::mousePressEvent(event);
        }
    }
};

#endif // MYQTREEVIEW_H
