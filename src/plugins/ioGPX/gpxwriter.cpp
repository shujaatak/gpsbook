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
#include "gpxwriter.h"
#include <QApplication>
#include <QStringList>
#include <QMessageBox>
#include <QObject>
#include <QVariant>

namespace PluginIOGPX {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeBounds(Bounds* bounds)
    {
        //qDebug()  << "writeBounds";
        writeStartElement("bounds");
            writeAttribute( "minlat", QString::number(bounds->minlat));
            writeAttribute( "minlon", QString::number(bounds->minlon));
            writeAttribute( "maxlat", QString::number(bounds->maxlat));
            writeAttribute( "maxlon", QString::number(bounds->maxlon));
        writeEndElement();
    } //GpxWriter::writeBounds

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeEMail(EMail* email)
    {
        //qDebug()  << "writeEMail";
        writeStartElement("email");
            if (email->id != "")
                writeTextElement("id",email->id);
            if (email->domain != "")
                writeTextElement("domain",email->domain);
        writeEndElement();
    } //GpxWriter::writeEMail

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeWayPoint(WayPoint* waypoint, QString name)
    {
        qApp->processEvents();
        //qDebug()  << "writeWayPoint";
        writeStartElement(name);
            writeAttribute( "lat", ((QVariant)waypoint->lat).toString());
            writeAttribute( "lon", ((QVariant)waypoint->lon).toString());
            if (waypoint->ele != 0)
                writeTextElement("ele",QString::number(waypoint->ele));
            if ( waypoint->time.isValid() )
                writeTextElement("time", waypoint->time.toString(Qt::ISODate));
            if (waypoint->magvar != 0)
                writeTextElement("magvar",QString::number(waypoint->magvar));
            if (waypoint->geoidheight != 0)
                writeTextElement("geoidheight",QString::number(waypoint->geoidheight));
            if (waypoint->name != "")
                writeTextElement("name",waypoint->name);
            if (waypoint->cmt != "")
                writeTextElement("cmt",waypoint->cmt);
            if (waypoint->desc != "")
                writeTextElement("desc",waypoint->desc);
            if (waypoint->src != "")
                writeTextElement("src",waypoint->src);
            foreach (Link* link, waypoint->linkList)
                writeLink(link);
            if (waypoint->sym != "")
                writeTextElement("sym",waypoint->sym);
            if (waypoint->type != "")
                writeTextElement("type",waypoint->type);
            if (waypoint->fix != "")
                writeTextElement("fix",waypoint->fix);
            if (waypoint->sat != 0)
                writeTextElement("sat",QString::number(waypoint->sat));
            if (waypoint->hdop != 0)
                writeTextElement("hdop",QString::number(waypoint->hdop));
            if (waypoint->vdop != 0)
                writeTextElement("vdop",QString::number(waypoint->vdop));
            if (waypoint->pdop != 0)
                writeTextElement("pdop",QString::number(waypoint->pdop));
            if (waypoint->ageofdgpsdata != 0)
                writeTextElement("ageofdgpsdata",QString::number(waypoint->ageofdgpsdata));
            if (waypoint->dgpsid != 0)
                writeTextElement("dgpsid",QString::number(waypoint->dgpsid));
            writeStartElement("extension");
                writeExtensions(waypoint->extensions);
            writeEndElement();
        writeEndElement();
    } //GpxWriter::writeWayPoint

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeExtensions(QVariantHash extension)
    {
        QHashIterator<QString, QVariant> iterator(extension);
        while (iterator.hasNext()) {
            iterator.next();
            writeStartElement(iterator.key());
                if ( iterator.value().type() == QVariant::Hash )
                {
                    //qDebug() << "Writer: " << iterator.key() << " = QHash(" << iterator.value().toHash().count() <<")";
                    writeExtensions(iterator.value().toHash());
                }
                else
                {
                    //qDebug() << "Writer: " << iterator.key() << " = " << iterator.value().toString();
                    writeAttribute( "value", iterator.value().toString());
                }
            writeEndElement();
        }
    } //GpxWriter::writeExtensions

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeRoute(Route* route)
    {
        //qDebug()  << "writeRoute";
        writeStartElement("rte");
            if (route->name != "")
                writeTextElement("name",route->name);
            if (route->cmt != "")
                writeTextElement("cmt",route->cmt);
            if (route->desc != "")
                writeTextElement("desc",route->desc);
            if (route->src != "")
                writeTextElement("src",route->src);
            foreach (Link* link, route->linkList)
                writeLink(link);
            if (route->number != 0)
                writeTextElement("number",QString::number(route->number));
            if (route->type != "")
                writeTextElement("type",route->type);
            foreach(WayPoint* wayPoint, route->wayPointList)
                writeWayPoint(wayPoint, "rtept");
            writeStartElement("extension");
                writeExtensions(route->extensions);
            writeEndElement();
        writeEndElement();
    } //GpxWriter::writeRoute

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeTrackSeg(TrackSeg* trackseg)
    {
        //qDebug()  << "writeTrackSeg";
        writeStartElement("trkseg");
            writeStartElement("extension");
                writeExtensions(trackseg->extensions);
            writeEndElement();
            foreach(WayPoint* wayPoint, trackseg->wayPointList)
                writeWayPoint(wayPoint, "trkpt");
        writeEndElement();
    } //GpxWriter::writeTrackSeg

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeTrack(Track* track)
    {
        //qDebug()  << "writeTrack";
        writeStartElement("trk");
            if (track->name != "")
                writeTextElement("name",track->name);
            if (track->cmt != "")
                writeTextElement("cmt",track->cmt);
            if (track->desc != "")
                writeTextElement("desc",track->desc);
            if (track->src != "")
                writeTextElement("src",track->src);
            foreach (Link* link, track->linkList)
                writeLink(link);
            if (track->number != 0)
                writeTextElement("number",QString::number(track->number));
            if (track->type != "")
                writeTextElement("type",track->type);
            writeStartElement("extension");
                writeExtensions(track->extensions);
            writeEndElement();
            foreach(TrackSeg* trackSeg, track->trackSegList)
                writeTrackSeg(trackSeg);
        writeEndElement();
    } //GpxWriter::writeTrack

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeLink(Link* link)
    {
         //qDebug()  << "writeLink";
         writeStartElement("link");
            if (link->href != "")
                writeTextElement("href",link->href);
            if (link->text != "")
                writeTextElement("text",link->text);
            if (link->type != "")
                writeTextElement("type",link->type);
         writeEndElement();
    } //GpxWriter::writeLink

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeCopyright(Copyright* copyright)
    {
        //qDebug()  << "writeCopyright";
        writeStartElement("copyright");
            if (copyright->author != "") {
                writeAttribute( "author", copyright->author);
            }
            writeTextElement("year",QString::number(copyright->year));
            if (copyright->license != "") {
                writeTextElement("license",copyright->license);
            }
        writeEndElement();
    } //GpxWriter::writeCopyright

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writePerson(Person* person)
    {
        //qDebug()  << "writePerson";
         writeStartElement("author");
             if (person->name != "") {
                writeTextElement("name",person->name);
             }
             writeEMail(person->email);
             writeLink(person->link);
         writeEndElement();
    } //GpxWriter::writePerson

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::writeMetaData(MetaData* metadata)
    {
        //qDebug()  << "writeMetaData";
        writeStartElement("metadata");
            if (metadata->name != "") {
                writeTextElement("name",metadata->name);
            }
            if (metadata->desc != "") {
                writeTextElement("desc",metadata->desc);
            }
            writePerson(metadata->author);
            writeCopyright(metadata->copyright);
            foreach (Link* link, metadata->linkList) {
                writeLink(link);
            }
            if (metadata->time.isValid()) {
                writeTextElement("time", metadata->time.toString(Qt::ISODate));
            }
            else {
                writeTextElement("time", "");
            }
            if (metadata->keywords != "") {
                writeTextElement("keywords",metadata->keywords);
            }
            writeBounds(metadata->bounds);
            writeStartElement("extension");
                writeExtensions(metadata->extensions);
            writeEndElement();
        writeEndElement();
    } //GpxWriter::writeMetaData


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    bool GpxWriter::load( GPSData* , QString )
    {
        return true;
    } //GpxWriter::load

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GpxWriter::save(GPSData* gpsdata, QString fileName )
    {
        //qDebug()  << "Save file into " << fileName;
        QFile file(fileName);
        setDevice(&file);

        gpsdata->lockGPSDataForRead();
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            //qDebug()  << "Error: Cannot write file ";
        }

        setAutoFormatting(true);
        writeStartDocument();
            writeStartElement("gpx");
            writeAttribute( "creator", "GPSBook");
            writeAttribute( "version", "1.1");
            writeNamespace( "http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd","schemaLocation");
            writeNamespace( "http://www.gpsbook.com/GPX/1/1 http://www.gpsbook.com/GPX/1/1/gpsbook.xsd","gpsbook");

            writeMetaData(gpsdata->metadata);

            foreach(WayPoint* wayPoint, gpsdata->wayPointList)
                writeWayPoint(wayPoint, "wpt");

            foreach(Route* route, gpsdata->routeList)
                writeRoute(route);

            foreach(Track* track, gpsdata->trackList)
                writeTrack(track);

        writeEndDocument();
        file.close();
        gpsdata->unlockGPSData();

        if (file.error())
        {
            //qDebug()  << "Error: Cannot write file ";
        }
        //qDebug() << "End store";
    } //GpxWriter::save
}
