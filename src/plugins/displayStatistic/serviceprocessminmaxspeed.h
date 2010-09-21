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
#ifndef SERVICEPROCESSSPEED_H
#define SERVICEPROCESSSPEED_H

#include <QObject>
#include "interface.h"
#include "gpsdata.h"

namespace PluginDisplayStatistic {

    class ServiceProcessMinMaxSpeed: public QThread
    {
        Q_OBJECT
    private:
        GPSData* mGPSData;
        double averageSpeed;
        double maximumSpeedX;
        double maximumSpeedY;
        double maximumSpeed;
        double average;
        double maxValueX;
        double maxValueY;
        double maxValue;
        int    nbpoints;

    public:
        ServiceProcessMinMaxSpeed();
        bool init(GPSData* aTracklist);
        void run();
        void getSpeedOfSegment(TrackSeg* trackseg);

    signals:
        void SignalAverageSpeedChanged(double speed);
        void SignalMaximumSpeedChanged(double speed, double longitude, double latitude);

    };
}


#endif // SERVICEPROCESSSPEED_H
