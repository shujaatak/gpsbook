#include <QtGlobal>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QCalendarWidget>
#else
#include <QCalendarWidget>
#endif

#include <QDebug>
#include <QStandardItemModel>
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

#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
        //Drag and drop between lists desactivated in Qt5
#else
        //qCritical() << event->format();
        if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {

            QStandardItemModel itemModel;
            itemModel.dropMimeData(event->mimeData(), Qt::CopyAction, 0,0, QModelIndex());
            QStandardItem* item = itemModel.takeItem(0,0);
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
#endif
    }
}
