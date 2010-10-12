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
#include "serviceprocesswaypointacceleration.h"



namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    ServiceProcessWaypointAcceleration::ServiceProcessWaypointAcceleration()
    {
    } //ServiceProcessWaypointAcceleration::ServiceProcessWaypointAcceleration

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    bool ServiceProcessWaypointAcceleration::init(GPSData* gpsdata)
    {
        mGPSData = gpsdata;
        return true;
    } //ServiceProcessWaypointAcceleration::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ServiceProcessWaypointAcceleration::run()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        WayPoint* waypointBegin = 0;
        WayPoint* waypointEnd = 0;
        double acceleration = 0;
        //mGPSData->lockGPSDataForWrite();
        foreach (Track* track, mGPSData->trackList)
        {
            foreach(TrackSeg* trackSeg, track->trackSegList)
            {
                mGPSData->setExtensionData(trackSeg->wayPointList[0]->extensions,"GPSBookWayPointExtension","acceleration",0);
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    if ( waypointBegin == 0 )
                        waypointEnd = waypoint;
                    waypointBegin = waypointEnd;
                    waypointEnd = waypoint;
                    uint timeBegin = waypointBegin->time.toTime_t();
                    uint timeEnd = waypointEnd->time.toTime_t();
                    double speedBegin = mGPSData->getExtensionData(waypointBegin->extensions,"GPSBookWayPointExtension","speed").toDouble();
                    double speedEnd = mGPSData->getExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","speed").toDouble();
                    double speedDelta = speedEnd - speedBegin;
                    uint time = timeEnd - timeBegin;
                    if ( time != 0 )
                    {
                        //Acceleration in m/s2
                        acceleration = 3600 * speedDelta / 1000 / time;
                        //qDebug() << "acceleration=" << acceleration;

                    }
                    waypointEnd->extensions = mGPSData->setExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","acceleration",acceleration);
                }
            }
        }
        mGPSData->unlockGPSData();
        mGPSData->setModified(true);
    } //ServiceProcessWaypointAcceleration::run
}


