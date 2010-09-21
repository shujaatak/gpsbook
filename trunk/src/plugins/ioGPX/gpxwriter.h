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

#ifndef GPXWRITER_H
#define GPXWRITER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QList>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDateTime>
#include "gpsdata.h"

using namespace GPSBook;

namespace PluginIOGPX {

    /**
      * class Gpx
      */
    class GpxWriter: public QXmlStreamWriter //, QObject
    {
        public:
            // Constructors/Destructors
            GpxWriter() {}
            virtual ~GpxWriter(){}
            void save( GPSData *gpsdata, QString fileName );
            bool load( GPSData* , QString );

        private:
            void writeBounds( Bounds* bounds );
            void writeEMail( EMail* email );
            void writeWayPoint( WayPoint* waypoint, QString name );
            void writeRoute( Route* route );
            void writeTrackSeg( TrackSeg* trackseg );
            void writeTrack( Track* track );
            void writeLink( Link* link );
            void writeCopyright( Copyright* copyright );
            void writePerson( Person* person );
            void writeExtensions( QVariantHash extension );
            void writeMetaData( MetaData* metadata );

    };
} //GpxWriter

#endif // GPXWRITER_H
