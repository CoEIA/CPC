/*
    CoEIA Privacy Control (CPC) - CPC provides easy and efficient methods of finding and
    erasing unwanted and unnecessary items on your computer.

    Copyright (C) 2014 Center of excellence in Information Assurance, All rights reserved.

    This program is free software; you can redistribute it and/or modify it under the terms
    of the GNU General Public License as published by the Free Software Foundation;
    either version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program;
    if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301 USA
*/

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
