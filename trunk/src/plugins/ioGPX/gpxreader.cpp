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
        QString flagName = qualifiedName().toString();
        while (!atEnd())
        {
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;

            if (qualifiedName() == "id")
            {
                email->id = readElementText();
            }
            if (qualifiedName() == "domain")
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
        QString flagName = qualifiedName().toString();
        while (!atEnd())
        {
            if ((qualifiedName() == "trkpt") or (qualifiedName() == "wpt") or (qualifiedName() == "rtept"))
            {
                waypoint->lat = attributes().value("lat").toString().toDouble();
                waypoint->lon = attributes().value("lon").toString().toDouble();
            }
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "WPT: = "<< qualifiedName().toString();
            if (qualifiedName() == "ele")
            {
                waypoint->ele = readElementText().toDouble();
            }
            if (qualifiedName() == "time")
            {
                waypoint->time = QDateTime::fromString(readElementText(),Qt::ISODate);
            }
            if (qualifiedName() == "magvar")
            {
                waypoint->magvar = readElementText().toDouble();
            }
            if (qualifiedName() == "geoidheight")
            {
                waypoint->geoidheight = readElementText().toDouble();
            }
            if (qualifiedName() == "name")
            {
                waypoint->name = readElementText();
            }
            if (qualifiedName() == "cmt")
            {
                waypoint->cmt = readElementText();
            }
            if (qualifiedName() == "desc")
            {
                waypoint->desc = readElementText();
                if (mCreator == "Nokia Sports Tracker") //Nokia Sports Tracker Customisation
                {
                    QStringList tmp = waypoint->desc.split(" ");
                    waypoint->extensions = mGPSData->setExtensionData(waypoint->extensions,"gpsbook:WayPointExtension","gpsbook:distance",tmp[tmp.indexOf("Distance")+1]);
                }
            }
            if (qualifiedName() == "src")
            {
                waypoint->src = readElementText();
            }
            if (qualifiedName() == "link")
            {
                Link* link = new Link();
                waypoint->linkList << link;
                readLink(link);
            }
            if (qualifiedName() == "sym")
            {
                waypoint->sym = readElementText();
            }
            if (qualifiedName() == "type")
            {
                waypoint->type = readElementText();
            }
            if (qualifiedName() == "fix")
            {
                waypoint->fix = readElementText();
            }
            if (qualifiedName() == "sat")
            {
                waypoint->sat = readElementText().toInt();
            }
            if (qualifiedName() == "hdop")
            {
                waypoint->hdop = readElementText().toDouble();
            }
            if (qualifiedName() == "vdop")
            {
                waypoint->vdop = readElementText().toDouble();
            }
             if (qualifiedName() == "pdop")
            {
                waypoint->pdop = readElementText().toDouble();
            }
            if (qualifiedName() == "ageofdgpsdata")
            {
                waypoint->ageofdgpsdata = readElementText().toDouble();
            }
            if (qualifiedName() == "dgpsid")
            {
                waypoint->dgpsid = readElementText().toInt();
            }
            //Nokia Sports Tracker Customisation
            if ((qualifiedName() == "speed") || (qualifiedName() == "course"))
            {
                waypoint->extensions = mGPSData->setExtensionData(waypoint->extensions,"gpsbook:WayPointExtension",qualifiedName().toString(),readElementText());
            }
            //Extensions to be added if needed
            if (qualifiedName() == "extension")
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
        //qDebug() << "readExtensions" << qualifiedName().toString() << "|" << prefix().toString() << "|" << namespaceUri();
        QString flagName = qualifiedName().toString();
        QVariantHash extension;
        while (!atEnd())
        {
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;

            if ( ( qualifiedName().toString() != "" ) and ( !isEndElement() ) )
            {
                QString value = attributes().value("value").toString();
                //qDebug() << "readExtensions" << qualifiedName().toString() << "=" << value;
                if ( value != "" )
                {
                    //qDebug() << "extension.insert(" << qualifiedName().toString() << "," << value << ");";
                    extension.insert(qualifiedName().toString(),value);
                }
                else
                {

                    //qDebug() << "extension.insert(" << qualifiedName().toString() << ", QVariantHash >>>>);";
                    extension.insert(qualifiedName().toString(), readExtensions());
                    //qDebug() << "extension.insert(" << qualifiedName().toString() << ", QVariantHash <<<<);";
                }
            }
        }
        return extension;
    } //GpxReader::readExtensions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readRoute(Route* route)
    {
        QString flagName = qualifiedName().toString();
        while (!atEnd())
        {
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "RTE: = "<< qualifiedName().toString();
            if (qualifiedName() == "name")
            {
                route->name = readElementText();
            }
            if (qualifiedName() == "cmt")
            {
                route->cmt = readElementText();
            }
            if (qualifiedName() == "desc")
            {
                route->desc = readElementText();
            }
            if (qualifiedName() == "src")
            {
                route->src = readElementText();
            }
            if (qualifiedName() == "link")
            {
                Link* link = new Link();
                route->linkList << link;
                readLink(link);
            }
            if (qualifiedName() == "number")
            {
                route->number = readElementText().toInt();
            }
            if (qualifiedName() == "type")
            {
                route->type = readElementText();
            }
            if (qualifiedName() == "rtept")
            {
                WayPoint* waypoint = new WayPoint();
                route->wayPointList << waypoint;
                readWayPoint(waypoint);
            }
            if (qualifiedName() == "extension")
            {
                route->extensions = readExtensions();
            }
        }
    } //GpxReader::readRoute

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readTrackSeg(TrackSeg* trackseg)
    {
        QString flagName = qualifiedName().toString();
        while (!atEnd())
        {
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "TRKPT: = "<< qualifiedName().toString();
            if (qualifiedName() == "trkpt")
            {
                WayPoint* waypoint = new WayPoint();
                trackseg->wayPointList << waypoint;
                readWayPoint(waypoint);
            }
            if (qualifiedName() == "extension")
            {
                trackseg->extensions = readExtensions();
            }
        }
    } //GpxReader::readTrackSeg

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readTrack(Track* track)
    {
        QString flagName = qualifiedName().toString();
        while (!atEnd())
        {
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "TRK: = "<< qualifiedName().toString();

            if (qualifiedName() == "name")
            {
                track->name = readElementText();
            }
            if (qualifiedName() == "cmt")
            {
                track->cmt = readElementText();
            }
            if (qualifiedName() == "desc")
            {
                track->desc = readElementText();
            }
            if (qualifiedName() == "src")
            {
                track->src = readElementText();
            }
            if (qualifiedName() == "type")
            {
                track->type = readElementText();
            }
            if (qualifiedName() == "number")
            {
                track->number = readElementText().toInt();
            }
            if (qualifiedName() == "link")
            {
                Link* link = new Link();
                track->linkList << link;
                readLink(link);
            }
            if (qualifiedName() == "trkseg")
            {
                TrackSeg* trackseg = new TrackSeg();
                track->trackSegList << trackseg;
                readTrackSeg(trackseg);
            }
            if (qualifiedName() == "extension")
            {
                track->extensions = readExtensions();
            }
        }
    } //GpxReader::readTrack

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readLink(Link* link)
    {
        QString flagName = qualifiedName().toString();
        while (!atEnd())
        {
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;
            if (qualifiedName() == "href")
            {
                link->href = readElementText();
            }
            if (qualifiedName() == "text")
            {
                link->text = readElementText();
            }
            if (qualifiedName() == "type")
            {
                link->type = readElementText();
            }
        }
    } //GpxReader::readLink

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readCopyright(Copyright* copyright)
    {
        QString flagName = qualifiedName().toString();
        while (!atEnd())
        {
            if ((qualifiedName() == "copyright"))
            {
                copyright->author = attributes().value("author").toString();
            }
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;

            if (qualifiedName() == "year")
            {
                copyright->year = readElementText().toInt();
            }
            if (qualifiedName() == "license")
            {
                copyright->license = readElementText();
            }
        }
    } //GpxReader::readCopyright

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readPerson(Person* person)
    {
        QString flagName = qualifiedName().toString();
        while (!atEnd())
        {
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "PER: = "<< qualifiedName().toString();
            if (qualifiedName() == "name")
            {
                person->name = readElementText();
            }
            if (qualifiedName() == "link")
            {
                readLink(person->link);
            }
            if (qualifiedName() == "email")
            {
                readEMail(person->email);
            }
        }
    } //GpxReader::readPerson

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxReader::readMetaData(MetaData* metadata)
    {
        QString flagName = qualifiedName().toString();
        while (!atEnd())
        {
            readNext();
            if ((qualifiedName().toString() == flagName) and (isEndElement()))
                break;

            //qDebug()<< "MET: = "<< qualifiedName().toString();

            if (qualifiedName() == "name")
            {
                metadata->name = readElementText();
            }
            if (qualifiedName() == "desc")
            {
                metadata->desc = readElementText();
            }
            if (qualifiedName() == "keywords")
            {
                metadata->keywords = readElementText();
            }
            if (qualifiedName() == "author")
            {
                readPerson(metadata->author);
            }
            if (qualifiedName() == "copyright")
            {
                readCopyright(metadata->copyright);
            }
            if (qualifiedName() == "link")
            {
                Link* link = new Link();
                metadata->linkList << link;
                readLink(link);
            }
            if (qualifiedName() == "time")
            {
                QString dateTimeStr = readElementText();
                if (dateTimeStr == "") {
                    metadata->time = QDateTime(QDate(0,0,0));
                }
                else {
                    metadata->time = QDateTime::fromString(dateTimeStr,Qt::ISODate);
                }
            }
            if (qualifiedName() == "bounds")
            {
                metadata->bounds->minlat = attributes().value("minlat").toString().toDouble();
                metadata->bounds->minlon = attributes().value("minlon").toString().toDouble();
                metadata->bounds->maxlat = attributes().value("maxlat").toString().toDouble();
                metadata->bounds->maxlon = attributes().value("maxlon").toString().toDouble();
            }
            if (qualifiedName() == "extension")
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
            //qDebug()<< "XML: = "<< qualifiedName().toString();
            if (qualifiedName() == "gpx")
            {
                QString flagName = qualifiedName().toString();
                while (!atEnd())
                {
                    if ((qualifiedName() == "gpx") && (isStartElement()))
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
                    if ((qualifiedName().toString() == flagName) and (isEndElement()))
                        break;

                    //qDebug()<< "GPX: = "<< qualifiedName().toString();

                    //-------------------------------------------------------------
                    if (qualifiedName() == "metadata")
                    {
                        mGPSData->metadata = new MetaData();
                        readMetaData(mGPSData->metadata);
                    }

                    //-------------------------------------------------------------
                    if (qualifiedName() == "wpt")
                    {
                        WayPoint* waypoint = new WayPoint();
                        mGPSData->wayPointList << waypoint;
                        readWayPoint(waypoint);
                    }

                    //-------------------------------------------------------------
                    if (qualifiedName() == "rte")
                    {
                        Route* route = new Route();
                        mGPSData->routeList << route;
                        readRoute(route);
                    }

                    //-------------------------------------------------------------
                    if (qualifiedName() == "trk")
                    {
                        Track* track = new Track();
                        mGPSData->trackList << track;
                        readTrack(track);

                    }

                    //-------------------------------------------------------------
                    if (qualifiedName() == "extension")
                    {
                        mGPSData->extensions = readExtensions();
                    }
                }
            }
            else
            {
                //qDebug()<< "Error: = "<<qualifiedName().toString();
            }
            readNext();
        }

        file.close();

        mGPSData->unlockGPSData();
        return true;
    } //GpxReader::load
}
