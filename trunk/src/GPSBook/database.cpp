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
#include <QDebug>
#include <QStringList>
#include <QObject>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QFile>
#include <QVariant>
#include <QTreeWidget>
#include <QDate>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QBrush>
#include <QFileInfo>
#include "database.h"
#include "gpsdata.h"

namespace GPSBook {

    /*------------------------------------------------------------------------------*
      Initialize a calandar widget with to highlight dates where traces has been
      saved.
     *------------------------------------------------------------------------------*/
    void Database::initCalendarWidget(QCalendarWidget* calendarWidget)
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        QTextCharFormat highlightFormat;
        QDate date;
        calendarWidget->setDateTextFormat( date, highlightFormat );
        highlightFormat.setBackground(QBrush(QColor ( 240, 240, 240, 255 )));
        calendarWidget->setWeekdayTextFormat( Qt::Saturday, highlightFormat);
        calendarWidget->setWeekdayTextFormat( Qt::Sunday, highlightFormat);

        highlightFormat.setFontWeight(QFont::Bold);
        highlightFormat.setFontItalic(true);
        highlightFormat.setBackground(QBrush(Qt::darkGreen));
        highlightFormat.setForeground(QBrush(Qt::white));

        QSqlDatabase db = QSqlDatabase::database();
        QString sql = "SELECT track_date FROM gpsbook_track";
        QSqlQuery queryGpsData(sql, db);
        while (queryGpsData.next())
        {
            if (queryGpsData.value(0).toString()=="")
                continue;
            QDateTime dateTime = QDateTime::fromString(queryGpsData.value(0).toString(),Qt::ISODate);
            calendarWidget->setDateTextFormat(dateTime.date(), highlightFormat);
        }
    } //Database::initCalendarWidget

    /*------------------------------------------------------------------------------*
      Get previous date with a trace stored in Catalog
     *------------------------------------------------------------------------------*/
    QDate Database::getPrevDate(QDate currentDate)
    {
        QSqlDatabase db = QSqlDatabase::database();
        QString sql = "SELECT track_date FROM gpsbook_track WHERE (track_date < \"" + currentDate.toString(Qt::ISODate) + "\") ORDER BY track_date DESC LIMIT 1";
        QSqlQuery queryGpsData(sql, db);
        if (queryGpsData.first())
          return queryGpsData.value(0).toDate();
        else
          return QDate();
    } //Database::getPrevDate

    /*------------------------------------------------------------------------------*
      Get next date with a trace stored in Catalog
     *------------------------------------------------------------------------------*/
    QDate Database::getNextDate(QDate currentDate)
    {
        QSqlDatabase db = QSqlDatabase::database();;
        QString sql = "SELECT track_date FROM gpsbook_track WHERE (track_date > \"" + currentDate.addDays(1).toString(Qt::ISODate) + "\") ORDER BY track_date ASC LIMIT 1";
        QSqlQuery queryGpsData(sql, db);
        if (queryGpsData.first())
          return queryGpsData.value(0).toDate();
        else
            return QDate();
    } //Database::getNextDate

    /*------------------------------------------------------------------------------*
      Update the treview with the trace of the specified date
     *------------------------------------------------------------------------------*/
    void Database::updateTreeWidget(QTreeWidget* treeWidget, QDate date)
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        treeWidget->clear();
        QSqlDatabase db = QSqlDatabase::database();;
        //Look into DB to find if file is not already in catalog
        QString datestr;
        if (date.isValid()){
            datestr = date.toString(Qt::ISODate)+"%";
        }
        QString sql = "SELECT track_filename, track_displayname, track_description FROM gpsbook_track WHERE (track_date like \""+datestr+"\")";
        QSqlQuery queryGpsData(sql, db);
        while (queryGpsData.next())
        {
            QTreeWidgetItem* itemGpsData = new QTreeWidgetItem();
            QString filename = queryGpsData.value(0).toString();

            itemGpsData->setData(0,Qt::UserRole,    queryGpsData.value(0).toString()); //filename
            QString displayName = queryGpsData.value(1).toString();
            if (displayName=="")
                displayName = QFileInfo(queryGpsData.value(0).toString()).fileName();
            itemGpsData->setData(0,Qt::UserRole + 1,displayName); //display name
            itemGpsData->setData(0,Qt::UserRole + 2,queryGpsData.value(2).toString()); //description

            //itemGpsData->setText(0,queryGpsData.value(1).toString()!=  "" ? queryGpsData.value(1).toString() : queryGpsData.value(0).toString());
            itemGpsData->setText(0,displayName);
            treeWidget->addTopLevelItem(itemGpsData);
            itemGpsData->setToolTip(0,queryGpsData.value(2).toString());
            itemGpsData->setIcon(0,QIcon(":/icons/silk/script.png"));
        }
    } //Database::updateTreeWidget

    /*------------------------------------------------------------------------------*
      Get md5sum signature of file (used ofr OLD storage based on file)
     *------------------------------------------------------------------------------*/
    QString Database::getMd5sumSignature(QString filename)
    {
        const int BLOCK_SIZE=1024*1024;
        QFile file(filename);
        QCryptographicHash md5Hash(QCryptographicHash::Md5);
        if (file.open(QIODevice::ReadOnly | QIODevice::Unbuffered))
        {
            while (!file.atEnd())
            {
                md5Hash.addData(file.read(BLOCK_SIZE));
            }
        }
        file.close();
        return md5Hash.result().toHex();
    } //Database::getMd5sumSignature

    /*------------------------------------------------------------------------------*
      Initialise Database and create tables if needed
     *------------------------------------------------------------------------------*/
    void Database::initializeDatabase()
    {
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query;

        if (!db.tables(QSql::Tables).contains("gpsbook_track"))
        {
            QString sql = "CREATE TABLE gpsbook_track ( track_filename TEXT primary key, "
                                                       "track_displayname TEXT, "
                                                       "track_date TEXT, "
                                                       "track_description TEXT, "
                                                       "track_md5sum TEXT )";
            query.exec(sql);
        }
    } //Database::initializeDatabase

    /*------------------------------------------------------------------------------*
      Add track into database
     *------------------------------------------------------------------------------*/
    void Database::addTrackInDatabase(GPSData* gpsdata)
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(QString::null, db);

        //Look into DB to find if file is not already in catalog
        QString sql = "SELECT * FROM gpsbook_track "
                      "WHERE track_filename=\""+gpsdata->filename+"\"";
        //File is in catalog
        if (query.exec(sql) && query.first())
        {
            //TODO: Check integrity ?
        }
        //File is not in catalog
        else
        {
            //Adding track in db
            sql = "INSERT INTO gpsbook_track (track_filename, track_md5sum, track_date) "
                                "VALUES(\""+QFile(gpsdata->filename).fileName()+"\", "
                                       "\""+Database::getMd5sumSignature(gpsdata->filename)+"\","
                                       "\""+gpsdata->metadata->time.toString(Qt::ISODate)+"\")";
            query.first();
            query.exec(sql);
        }
    } //Database::addTrackInDatabase


    /*------------------------------------------------------------------------------*
      Add file into database
     *------------------------------------------------------------------------------*/
    void Database::addFileInDatabase(QString filename)
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        QSqlDatabase db = QSqlDatabase::database();
        QSqlQuery query(QString::null, db);

        //Look into DB to find if file is not already in catalog
        QString sql = "SELECT * FROM gpsbook_track "
                      "WHERE track_filename=\""+filename+"\"";
        //File is in catalog
        if (query.exec(sql) && query.first())
        {
            //TODO: Check integrity ?
        }
        //File is not in catalog
        else
        {
            //Adding track in db
            sql = "INSERT INTO gpsbook_track (track_filename, track_md5sum, track_date) "
                                "VALUES(\""+QFile(filename).fileName()+"\", "
                                       "\""+Database::getMd5sumSignature(filename)+"\","
                                       "\"\")";
            query.first();
            query.exec(sql);
        }
    } //Database::addTrackInDatabase

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Database::updateTrackProperties(QString filename, QString displayName, QString description)
    {
        QSqlQuery query;
        QString sql = "UPDATE gpsbook_track SET  track_displayname=\"" + displayName + "\", "
                                                "track_description=\"" + description + "\" "
                      "WHERE track_filename = \"" + QFile(filename).fileName() + "\"";
        query.exec(sql);
    } //Database::updateTrackProperties

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Database::updateDate(QString filename, QDate date)
    {
        QSqlQuery query;
        QString sql = "UPDATE gpsbook_track SET  track_date=\"" + date.toString(Qt::ISODate) + "\" "
                      "WHERE track_filename = \"" + QFile(filename).fileName() + "\"";
        query.exec(sql);
    } //Database::updateDate

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void Database::deleteTrack(QString filename)
    {
        QSqlQuery query;
        QString sql = "DELETE FROM gpsbook_track "
                      "WHERE track_filename = \"" + QFile(filename).fileName() + "\"";
        query.exec(sql);
        QFile::remove(filename);
    } //Database::deleteTrack
}
