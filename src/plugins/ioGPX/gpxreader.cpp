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
#include "gpxreader.h"
#include <QStringList>
#include <QApplication>
#include <QMessageBox>
#include <QObject>

namespace PluginIOGPX {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readEMail(EMail* email)
    {
        QString flagName = name().toString();
        while (!atEnd())
        {
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;

            if (name() == "id")
            {
                email->id = readElementText();
            }
            if (name() == "domain")
            {
                email->domain = readElementText();
            }
        }
    } //GpxReader::readEMail

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readWayPoint(WayPoint* waypoint)
    {
        qApp->processEvents();
        QString flagName = name().toString();
        while (!atEnd())
        {
            if ((name() == "trkpt") or (name() == "wpt") or (name() == "rtept"))
            {
                waypoint->lat = attributes().value("lat").toString().toDouble();
                waypoint->lon = attributes().value("lon").toString().toDouble();
            }
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "WPT: = "<< name().toString();
            if (name() == "ele")
            {
                waypoint->ele = readElementText().toDouble();
            }
            if (name() == "time")
            {
                waypoint->time = QDateTime::fromString(readElementText(),Qt::ISODate);
            }
            if (name() == "magvar")
            {
                waypoint->magvar = readElementText().toDouble();
            }
            if (name() == "geoidheight")
            {
                waypoint->geoidheight = readElementText().toDouble();
            }
            if (name() == "name")
            {
                waypoint->name = readElementText();
            }
            if (name() == "cmt")
            {
                waypoint->cmt = readElementText();
            }
            if (name() == "desc")
            {
                waypoint->desc = readElementText();
                if (mCreator == "Nokia Sports Tracker") //Nokia Sports Tracker Customisation
                {
                    QStringList tmp = waypoint->desc.split(" ");
                    waypoint->extensions = mGPSData->setExtensionData(waypoint->extensions,"GPSBookWayPointExtension","distance",tmp[tmp.indexOf("Distance")+1]);
                }
            }
            if (name() == "src")
            {
                waypoint->src = readElementText();
            }
            if (name() == "link")
            {
                Link* link = new Link();
                waypoint->linkList << link;
                readLink(link);
            }
            if (name() == "sym")
            {
                waypoint->sym = readElementText();
            }
            if (name() == "type")
            {
                waypoint->type = readElementText();
            }
            if (name() == "fix")
            {
                waypoint->fix = readElementText();
            }
            if (name() == "sat")
            {
                waypoint->sat = readElementText().toInt();
            }
            if (name() == "hdop")
            {
                waypoint->hdop = readElementText().toDouble();
            }
            if (name() == "vdop")
            {
                waypoint->vdop = readElementText().toDouble();
            }
             if (name() == "pdop")
            {
                waypoint->pdop = readElementText().toDouble();
            }
            if (name() == "ageofdgpsdata")
            {
                waypoint->ageofdgpsdata = readElementText().toDouble();
            }
            if (name() == "dgpsid")
            {
                waypoint->dgpsid = readElementText().toInt();
            }
            //Nokia Sports Tracker Customisation
            if ((name() == "speed") || (name() == "course"))
            {
                waypoint->extensions = mGPSData->setExtensionData(waypoint->extensions,"GPSBookWayPointExtension",name().toString(),readElementText());
            }
            //Extensions to be added if needed
            if (name() == "extension")
            {
                waypoint->extensions = readExtensions();
            }
        }
    } //GpxReader::readWayPoint

    /*------------------------------------------------------------------------------*
      TODO add description!!
     *------------------------------------------------------------------------------*/
    QVariantHash GpxReader::readExtensions()
    {
        QString flagName = name().toString();
        QVariantHash extension;
        while (!atEnd())
        {
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;

            if ( ( name().toString() != "" ) and ( !isEndElement() ) )
            {
                QString value = attributes().value("value").toString();
                //qDebug() << "readExtensions" << name().toString() << value;
                if ( value != "" )
                {
                    //qDebug() << "extension.insert(" << name().toString() << "," << value << ");";
                    extension.insert(name().toString(),value);
                }
                else
                {

                    //qDebug() << "extension.insert(" << name().toString() << ", QVariantHash >>>>);";
                    extension.insert(name().toString(), readExtensions());
                    //qDebug() << "extension.insert(" << name().toString() << ", QVariantHash <<<<);";

                }
            }
        }
        return extension;
    } //GpxReader::readExtensions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readRoute(Route* route)
    {
        QString flagName = name().toString();
        while (!atEnd())
        {
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "RTE: = "<< name().toString();
            if (name() == "name")
            {
                route->name = readElementText();
            }
            if (name() == "cmt")
            {
                route->cmt = readElementText();
            }
            if (name() == "desc")
            {
                route->desc = readElementText();
            }
            if (name() == "src")
            {
                route->src = readElementText();
            }
            if (name() == "link")
            {
                Link* link = new Link();
                route->linkList << link;
                readLink(link);
            }
            if (name() == "number")
            {
                route->number = readElementText().toInt();
            }
            if (name() == "type")
            {
                route->type = readElementText();
            }
            if (name() == "rtept")
            {
                WayPoint* waypoint = new WayPoint();
                route->wayPointList << waypoint;
                readWayPoint(waypoint);
            }
            if (name() == "extension")
            {
                route->extensions = readExtensions();
            }
        }
    } //GpxReader::readRoute

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readTrackSeg(TrackSeg* trackseg)
    {
        QString flagName = name().toString();
        while (!atEnd())
        {
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "TRS: = "<< name().toString();
            if (name() == "trkpt")
            {
                WayPoint* waypoint = new WayPoint();
                trackseg->wayPointList << waypoint;
                readWayPoint(waypoint);
            }
            if (name() == "extension")
            {
                trackseg->extensions = readExtensions();
            }
        }
    } //GpxReader::readTrackSeg

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readTrack(Track* track)
    {
        QString flagName = name().toString();
        while (!atEnd())
        {
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "TRK: = "<< name().toString();

            if (name() == "name")
            {
                track->name = readElementText();
            }
            if (name() == "cmt")
            {
                track->cmt = readElementText();
            }
            if (name() == "desc")
            {
                track->desc = readElementText();
            }
            if (name() == "src")
            {
                track->src = readElementText();
            }
            if (name() == "type")
            {
                track->type = readElementText();
            }
            if (name() == "number")
            {
                track->number = readElementText().toInt();
            }
            if (name() == "link")
            {
                Link* link = new Link();
                track->linkList << link;
                readLink(link);
            }
            if (name() == "trkseg")
            {
                TrackSeg* trackseg = new TrackSeg();
                track->trackSegList << trackseg;
                readTrackSeg(trackseg);
            }
            if (name() == "extension")
            {
                track->extensions = readExtensions();
            }
        }
    } //GpxReader::readTrack

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readLink(Link* link)
    {
        QString flagName = name().toString();
        while (!atEnd())
        {
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;
            if (name() == "href")
            {
                link->href = readElementText();
            }
            if (name() == "text")
            {
                link->text = readElementText();
            }
            if (name() == "type")
            {
                link->type = readElementText();
            }
        }
    } //GpxReader::readLink

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readCopyright(Copyright* copyright)
    {
        QString flagName = name().toString();
        while (!atEnd())
        {
            if ((name() == "copyright"))
            {
                copyright->author = attributes().value("author").toString();
            }
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;

            if (name() == "year")
            {
                copyright->year = readElementText().toInt();
            }
            if (name() == "license")
            {
                copyright->license = readElementText();
            }
        }
    } //GpxReader::readCopyright

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readPerson(Person* person)
    {
        QString flagName = name().toString();
        while (!atEnd())
        {
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "PER: = "<< name().toString();
            if (name() == "name")
            {
                person->name = readElementText();
            }
            if (name() == "link")
            {
                readLink(person->link);
            }
            if (name() == "email")
            {
                readEMail(person->email);
            }
        }
    } //GpxReader::readPerson

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readMetaData(MetaData* metadata)
    {
        QString flagName = name().toString();
        while (!atEnd())
        {
            readNext();
            if ((name().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "MET: = "<< name().toString();

            if (name() == "name")
            {
                metadata->name = readElementText();
            }
            if (name() == "desc")
            {
                metadata->desc = readElementText();
            }
            if (name() == "keywords")
            {
                metadata->keywords = readElementText();
            }
            if (name() == "author")
            {
                readPerson(metadata->author);
            }
            if (name() == "copyright")
            {
                readCopyright(metadata->copyright);
            }
            if (name() == "link")
            {
                Link* link = new Link();
                metadata->linkList << link;
                readLink(link);
            }
            if (name() == "time")
            {
                QString dateTimeStr = readElementText();
                if (dateTimeStr == "") {
                    metadata->time = QDateTime(QDate(0,0,0));
                }
                else {
                    metadata->time = QDateTime::fromString(dateTimeStr,Qt::ISODate);
                }
            }
            if (name() == "bounds")
            {
                metadata->bounds->minlat = attributes().value("minlat").toString().toDouble();
                metadata->bounds->minlon = attributes().value("minlon").toString().toDouble();
                metadata->bounds->maxlat = attributes().value("maxlat").toString().toDouble();
                metadata->bounds->maxlon = attributes().value("maxlon").toString().toDouble();
            }
            if (name() == "extension")
            {
                metadata->extensions = readExtensions();
            }
        }
    } //GpxReader::readMetaData

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    bool GpxReader::load(GPSData* gpsdata, QString fileName )
    {
        mGPSData = gpsdata;

        //mGPSData->lockGPSDataForWrite();

        mGPSData->clearData();

        mGPSData->filename = fileName;

        QFile file(fileName);

        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
             QMessageBox::critical(NULL,QObject::tr("Open GPX File"),QObject::tr("Unable to load file."));
             return false;
        }
        setDevice(&file);

        //qDebug()<< "GPX - Parsing file: " << fileName;
        while (!atEnd())
        {
            //qDebug()<< "XML: = "<< name().toString();
            if (name() == "gpx")
            {
                QString flagName = name().toString();
                while (!atEnd())
                {
                    if ((name() == "gpx") && (isStartElement()))
                    {
                        mCreator = attributes().value("creator").toString();
                        mGPSData->creator = mCreator;
                        mGPSData->version = attributes().value("version").toString();
                        if ( mGPSData->version != "1.1")
                        {
                            QMessageBox::information(NULL,QObject::tr("GPX file loader"),QObject::tr("GPX plugin is designed to load GPX v1.1. Some information of this file may not be loaded correctly."));
                        }
                    }
                    readNext();
                    if ((name().toString() == flagName) and (isEndElement()))
                        break;

                    //qDebug()<< "GPX: = "<< name().toString();

                    //-------------------------------------------------------------
                    if (name() == "metadata")
                    {
                        mGPSData->metadata = new MetaData();
                        readMetaData(mGPSData->metadata);
                    }

                    //-------------------------------------------------------------
                    if (name() == "wpt")
                    {
                        WayPoint* waypoint = new WayPoint();
                        mGPSData->wayPointList << waypoint;
                        readWayPoint(waypoint);
                    }

                    //-------------------------------------------------------------
                    if (name() == "rte")
                    {
                        Route* route = new Route();
                        mGPSData->routeList << route;
                        readRoute(route);
                    }

                    //-------------------------------------------------------------
                    if (name() == "trk")
                    {
                        Track* track = new Track();
                        mGPSData->trackList << track;
                        readTrack(track);

                    }

                    //-------------------------------------------------------------
                    if (name() == "extension")
                    {
                        mGPSData->extensions = readExtensions();
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
    } //GpxReader::load
}
