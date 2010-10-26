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

#ifndef LOCREADER_H
#define LOCREADER_H

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

namespace PluginIOLOC {

    /**
      * class LocReader
      */
    class LocReader: public QXmlStreamReader
    {
        public:
            // Constructors/Destructors
            LocReader(){}
            virtual ~LocReader(){}
            bool load( GPSData *gpsdata, QString fileName );
            void save( GPSData , QString ){}

        private:
            GPSData* mGPSData;
    };
} //LocReader

#endif // LOCREADER_H
