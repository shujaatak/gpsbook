/****************************************************************************
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor,
** Boston, MA  02110-1301, USA.
**
** ---
** Copyright (C) 2009, gpsbook-team
**
****************************************************************************/
#ifndef DATABASE_H
#define DATABASE_H
#include <QObject>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QListWidget>
#else
#include <QCalendarWidget>
#include <QListWidget>
#endif

#include <QDate>
#include <QSqlQuery>
#include "gpsdata.h"

namespace GPSBook {

    class Database : public QObject
    {
    Q_OBJECT

    public:
        static void initializeDatabase();
        static void addTrackInDatabase(GPSData* gpsdata);
        static void addFileInDatabase(QString filename);
        static QString getMd5sumSignature(QString filename);
        static void updateListWidget(QListWidget* listWidget, QDate date);
        static QDate getPrevDate(QDate currentDate);
        static QDate getNextDate(QDate currentDate);
        static void initCalendarWidget(QCalendarWidget* calendarWidget);
        static void deleteTrack(QString filename);
        static void updateTrackProperties(QString filename, QString displayName, QString description);
        static void updateDate(QString filename, QDate date);
        static QSqlDatabase database();


    //signals:
    //    void signalDatabaseUpdated();
    };
}

#endif // DATABASE_H
