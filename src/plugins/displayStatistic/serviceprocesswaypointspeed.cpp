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
#include "serviceprocesswaypointspeed.h"



namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    ServiceProcessWaypointSpeed::ServiceProcessWaypointSpeed()
    {
    } //ServiceProcessWaypointSpeed::ServiceProcessWaypointSpeed

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    bool ServiceProcessWaypointSpeed::init(GPSData* gpsdata)
    {
        mGPSData = gpsdata;
        return true;
    } //ServiceProcessWaypointSpeed::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void ServiceProcessWaypointSpeed::run()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        WayPoint* waypointBegin = 0;
        WayPoint* waypointEnd = 0;
        double speed = 0;

        //mGPSData->lockGPSDataForWrite();
        foreach (Track* track, mGPSData->trackList)
        {
            foreach(TrackSeg* trackSeg, track->trackSegList)
            {
                mGPSData->setExtensionData(trackSeg->wayPointList[0]->extensions,"GPSBookWayPointExtension","speed",0);
                foreach (WayPoint* waypoint, trackSeg->wayPointList)
                {
                    if ( waypointBegin == 0 )
                        waypointEnd = waypoint;
                    waypointBegin = waypointEnd;
                    waypointEnd = waypoint;
                    uint timeBegin = waypointBegin->time.toTime_t();
                    uint timeEnd = waypointEnd->time.toTime_t();
                    double distanceBegin = mGPSData->getExtensionData(waypointBegin->extensions,"GPSBookWayPointExtension","distance").toDouble();
                    double distanceEnd = mGPSData->getExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","distance").toDouble();
                    double distance = distanceEnd - distanceBegin;
                    uint time = timeEnd - timeBegin;
                    //qDebug() << "Distance=" << distance << "; time=" << time;
                    speed=0;
                    if ( time != 0 )
                    {
                        //speed in km/h
                        speed = ( 3600 * distance ) / time ;
                        //double delta = prevSpeed - speed;
                        //qDebug() << "Speed=" << speed;

                        //If speed difference between 2 point is too high, we filter it!
                        //if (abs(delta) < 33)
                        //{
                        //    mGPSData->setExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","speed",speed);
                        //    prevSpeed = speed;
                        //}
                        //else
                        //    mGPSData->setExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","speed",prevSpeed);
                    }
                    //else
                    waypointEnd->extensions = mGPSData->setExtensionData(waypointEnd->extensions,"GPSBookWayPointExtension","speed",speed);
                }
            }   
        }
        mGPSData->unlockGPSData();
        mGPSData->setModified(true);
    } //ServiceProcessWaypointSpeed::run

}
