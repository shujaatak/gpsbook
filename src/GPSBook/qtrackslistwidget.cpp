#include <QDebug>
#include <QStandardItemModel>
#include <QCalendarWidget>
#include "qtrackslistwidget.h"
#include "database.h"

namespace GPSBook {

    QTracksListWidget::QTracksListWidget(QWidget *parent) :
        QListWidget(parent)
    {
    }

    void QTracksListWidget::dropEvent(QDropEvent *event)
    {
        QListWidget::dropEvent(event);
        qCritical() << event->format();
        if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {

            QStandardItemModel itemModel;
            itemModel.dropMimeData(event->mimeData(), Qt::CopyAction, 0,0, QModelIndex());
            QStandardItem* item = itemModel.takeItem(0,0);
            qDebug() << "a" << item->text();
            qCritical() << "z" << item->data(Qt::UserRole).toString();
            qCritical() << "e" << item->data(Qt::UserRole+1).toString();
            qCritical() << "r" << event->source()->objectName();
            //If source is no date list, then add date to the track
            if (event->source()->objectName() == "listWidgetNoDateTracks")
            {
                QCalendarWidget* calendarWidget = parentWidget()->findChild<QCalendarWidget *>("calendarWidget");
                Database::updateDate(item->data(Qt::UserRole).toString(),calendarWidget->selectedDate());
            }

            //If source is list of track with date, then remove the date
            if (event->source()->objectName() == "listWidgetTracksOfTheDay")
            {
                Database::updateDate(item->data(Qt::UserRole).toString(),QDate(0,0,0));
            }
        }
    }
}
