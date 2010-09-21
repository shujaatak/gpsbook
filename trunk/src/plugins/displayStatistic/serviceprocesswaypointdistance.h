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
** Copyright (c) 2010, gpsbook-team
**
****************************************************************************/
#ifndef SERVICEPROCESSWAYPOINTDISTANCE_H
#define SERVICEPROCESSWAYPOINTDISTANCE_H
#include <QObject>
#include <QDebug>
#include <math.h>
#include "interface.h"
#include "gpsdata.h"

namespace PluginDisplayStatistic {

    class ServiceProcessWaypointDistance: public QThread
    {
        Q_OBJECT
    private:
        GPSData* mGPSData;
        double calculateDistance(double lon1, double lat1, double lon2, double lat2);

    public:
        ServiceProcessWaypointDistance();
        bool init(GPSData* gpsdata);
        void run();

    //signals:

    };
}

#endif // SERVICEPROCESSWAYPOINTDISTANCE_H
