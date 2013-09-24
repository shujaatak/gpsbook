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
#include "locreader.h"
#include <QStringList>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#else
#include <QApplication>
#include <QMessageBox>
#endif
#include <QObject>

namespace PluginIOLOC {

    /*
    <?xml version="1.0" encoding="UTF-8"?>
    <loc version="1.0" src="Groundspeak">
        <waypoint>
                <name id="GCHJM7"><![CDATA[Gotcha-Musk by basill]]></name>
                <coord lat="45.19835" lon="6.71935"/>
                <type>Geocache</type>
                <link text="Cache Details">http://www.geocaching.com/seek/cache_details.aspx?wp=GCHJM7</link>
        </waypoint>
    </loc>
    */

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    bool LocReader::load(GPSData* gpsdata, QString fileName )
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData = gpsdata;

        //mGPSData->lockGPSDataForWrite();

        mGPSData->clearData();

        mGPSData->filename = fileName;

        QFile file(fileName);

        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
             QMessageBox::critical(NULL,QObject::tr("Open LOC File"),QObject::tr("Unable to load file."));
             return false;
        }
        setDevice(&file);

        qDebug()<< "LOC - Parsing file: " << fileName;
        while (!atEnd())
        {
            qDebug()<< "XML: = "<< name().toString();
            if (name() == "loc")
            {
                QString flagName = name().toString();
                while (!atEnd())
                {
                    if ((name() == "loc") && (isStartElement()))
                    {
                        //mSrc = attributes().value("src").toString();
                    }
                    readNext();
                    if ((name().toString() == flagName) and (isEndElement()))
                        break;

                    qDebug()<< "LOC: = "<< name().toString();

                    //-------------------------------------------------------------
                    if (name() == "waypoint")
                    {
                        WayPoint* waypoint = new WayPoint();
                        mGPSData->wayPointList << waypoint;
                        QString flagName = name().toString();
                        while (!atEnd())
                        {
                            readNext();
                            if ((name().toString() == flagName) and (isEndElement()))
                                break;
                            if (name() == "coord")
                            {
                                waypoint->lat = attributes().value("lat").toString().toDouble();
                                waypoint->lon = attributes().value("lon").toString().toDouble();
                                waypoint->sym = "Geocache";
                                //qDebug()<< "WAYPOINT = "<< name().toString() <<  waypoint->lon << "/" << waypoint->lat;
                                readNext();
                            }
                            if (name() == "type")
                            {
                                waypoint->type = readElementText();
                                //qDebug()<< "WAYPOINT = "<< name().toString() <<  waypoint->type ;
                            }
                            if (name() == "name")
                            {
                                waypoint->cmt = attributes().value("id").toString();
                                waypoint->name = readElementText().replace(QString("\n"),"");
                                waypoint->desc = waypoint->name;
                                //qDebug()<< "WAYPOINT = "<< name().toString() <<  waypoint->name << "/" << waypoint->cmt;
                            }
                            if (name() == "link")
                            {
                                Link* link = new Link();
                                waypoint->linkList << link;
                                link->text = attributes().value("text").toString();
                                link->href = readElementText();
                                //qDebug()<< "WAYPOINT = "<< name().toString() <<  link->text << "/" << link->href;
                            }
                        }
                    }
                }
            }
            else
            {
                //qDebug()<< "Error: = "<<name().toString();
            }
            readNext();
        }

        file.close();

        mGPSData->unlockGPSData();
        return true;
    } //LocReader::load
}
