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
#ifndef SERVICEPROCESSMINMAXALTITUDE_H
#define SERVICEPROCESSMINMAXALTITUDE_H

#include <QObject>
#include "interface.h"
#include "gpsdata.h"

namespace PluginDisplayStatistic {

    class ServiceProcessMinMaxAltitude : public QThread
    {
        Q_OBJECT
    private:
        GPSData* gpsdata;
        double minimumAltitudeX;
        double minimumAltitudeY;
        double minimumAltitudeZ;
        double maximumAltitudeX;
        double maximumAltitudeY;
        double maximumAltitudeZ;
        double minTmpValueX;
        double minTmpValueY;
        double minTmpValueZ;
        double maxTmpValueX;
        double maxTmpValueY;
        double maxTmpValueZ;
        void getAltitudesOfSegment(TrackSeg* trackseg);
    public:
        ServiceProcessMinMaxAltitude();
        bool init(GPSData* aTracklist);
        void run();

    signals:
        void SignalMinimumAltitudeChanged(double altitude, double longitude, double latitude);
        void SignalMaximumAltitudeChanged(double altidude, double longitude, double latitude);

    };
}

#endif // SERVICEPROCESSMINMAXALTITUDE_H
